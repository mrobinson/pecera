#include "PeceraApplication.h"
#include "Project.h"
#include "Result.h"
#include "SearchBar.h"
#include "SearchProvider.h"
#include "SuggestionBox.h"
#include "Window.h"
#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QPaintEvent>
#include <algorithm>

namespace Pecera
{

SuggestionBox::SuggestionBox(SearchBar* bar)
    : QFrame(0)
    , m_bar(bar)
    , m_searchTask(0)
    , m_activeIndex(0)
    , m_activeResult(0)
    , m_prelightIndex(-1)
    , m_shouldStartNewSearchWhenLineEditChanges(true)
    , m_normalFont(font())
    , m_boldFont(font())
    , m_normalFontMetrics(0)
    , m_boldFontMetrics(0)
    , m_paintTimer(this)
    , m_backBuffer(1000, 1000)
{
    setWindowFlags(Qt::Popup);
    setFocusPolicy(Qt::NoFocus);
    setFocusProxy(m_bar);
    setMouseTracking(true);

    installEventFilter(this);

    m_boldFont.setBold(true);
    m_normalFontMetrics = new QFontMetrics(m_normalFont);
    m_boldFontMetrics = new QFontMetrics(m_boldFont);

    connect(this, SIGNAL(forcePaint()),
        this, SLOT(paintTimeout()));
    connect(&m_paintTimer, SIGNAL(timeout()),
        this, SLOT(paintTimeout()));
    setAttribute(Qt::WA_ShowWithoutActivating);
}

void SuggestionBox::setActiveIndex(int newActiveIndex)
{
    m_prelightIndex = -1;
    {
        QMutexLocker lock(m_searchTask->resultsMutex());
        if (newActiveIndex < 0)
            newActiveIndex = 0;
        if (newActiveIndex > m_searchTask->results().size() - 1)
            newActiveIndex = m_searchTask->results().size() - 1;

        m_activeResult = m_searchTask->results()[newActiveIndex]; 

        m_shouldStartNewSearchWhenLineEditChanges = false;
        m_bar->setText(m_activeResult->url());
        m_shouldStartNewSearchWhenLineEditChanges = true;

        m_activeIndex = newActiveIndex;
    }
    emit forcePaint();
}

bool SuggestionBox::eventFilter(QObject*, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == Qt::Key_Up) {
            setActiveIndex(m_activeIndex - 1);
            return true;
        }

        if (keyEvent->key() == Qt::Key_Down) {
            setActiveIndex(m_activeIndex + 1);
            return true;
        }

        if (keyEvent->key() == Qt::Key_L && keyEvent->modifiers() & Qt::ControlModifier) {
            PeceraApplication::instance()->window()->focusSearchBar();
            return true;
        }

        if (keyEvent->key() == Qt::Key_Escape) {
            stopSearchAndHide();
        }

        m_bar->setFocus();
        m_bar->event(event);
    }

    return false;
}

void SuggestionBox::mouseMoveEvent(QMouseEvent* event)
{
    m_prelightIndex = event->y() / getLineHeight();
}

void SuggestionBox::mousePressEvent(QMouseEvent* event)
{
    // Outside the box. Hide.
    if (event->x() < 0 || event->x() > width() || event->y() < 0 || event->y() > height()) {
        stopSearchAndHide();
        m_bar->setFocus();
        return;
    }

    setActiveIndex(event->y() / getLineHeight());
    returned();

}

void SuggestionBox::stopSearchAndHide()
{
    m_shouldStartNewSearchWhenLineEditChanges = false;
    m_bar->setText(m_originalSearchBarText);
    m_shouldStartNewSearchWhenLineEditChanges = true;

    if (m_searchTask) {
        m_searchTask->stop();
        m_searchTask = 0;
    }
    hide();
}

SuggestionBox::~SuggestionBox()
{
    if (m_boldFontMetrics)
        delete m_boldFontMetrics;
    if (m_normalFontMetrics)
        delete m_normalFontMetrics;
}

bool SuggestionBox::newSearchResult(SearchTask* task)
{
    if (task != m_searchTask)
        return false;

    if (!m_activeResult)
        setActiveIndex(0);

    return m_searchTask->results().size() <= 10;
}

void SuggestionBox::searchComplete(SearchTask* task)
{
    if (task != m_searchTask)
        return;

    m_paintTimer.stop();
    emit forcePaint();
}

void SuggestionBox::paintTimeout()
{
    if (!m_searchTask) {
        hide();
        return;
    }

    int size = 0;
    {
        QMutexLocker lock(m_searchTask->resultsMutex());
        size = m_searchTask->results().size();
    }

    if (size == 0) {
        hide();
        return;
    }

    const QRect& geometry = m_bar->geometry();
    const QPoint& point = m_bar->mapToGlobal(QPoint(0, geometry.height()));
    if (!isVisible()) {
        this->setGeometry(point.x(), point.y(), geometry.width(), getLineHeight() * (size + 1));
        QWidget* mainWindow = m_bar->window();
        show();
    } else {
        this->resize(geometry.width(), getLineHeight() * (size + 1));
        repaint(rect());
    }
}

void SuggestionBox::paintEvent(QPaintEvent* event)
{
    if (isHidden() || !m_searchTask)
        return;

    QPalette palette(QApplication::palette());
    QPainter painter(&m_backBuffer);
    painter.fillRect(event->rect(), palette.background());

    int lineHeight = getLineHeight();
    int baselineOffset = (lineHeight / 2) + (m_normalFontMetrics->ascent() / 2);
    int currentLineOffset = 0;

    QMutexLocker lock(m_searchTask->resultsMutex());
    for (int i = 0; i < m_searchTask->results().size(); i++) {
        if (i == m_activeIndex) {
            painter.fillRect(QRect(0, currentLineOffset, event->rect().width(), lineHeight), palette.highlight());
            painter.setBrush(palette.highlightedText());
        } else if (i == m_prelightIndex) {
            QBrush prelightBrush(QColor(
                (palette.highlight().color().red() + palette.background().color().red()) / 2,
                (palette.highlight().color().green() + palette.background().color().green()) / 2,
                (palette.highlight().color().blue() + palette.background().color().blue()) / 2));
            painter.fillRect(QRect(0, currentLineOffset, event->rect().width(), lineHeight), prelightBrush);
            painter.setBrush(palette.highlightedText());
        } else {
            painter.setBrush(palette.text());
        }

        Result* result = m_searchTask->results()[i];
        const QString& text = result->text();

        int baseline = currentLineOffset + baselineOffset;
        int currentIndex = 0;
        int currentX = 0;
        const QList<Pecera::Extent>& extents = result->extents();
        for (int j = 0; j < extents.size(); ++j) {
            const Pecera::Extent& extent = extents[j];

            QString beforeExtent = text.mid(currentIndex, extent.start() - currentIndex);
            if (beforeExtent.size() > 0) {
                painter.drawText(currentX, baseline, beforeExtent);
                currentX += m_normalFontMetrics->width(beforeExtent);
            }

            QString extentText = text.mid(extent.start(), extent.length());
            painter.setFont(m_boldFont);
            painter.drawText(currentX, baseline, extentText);
            currentX += m_boldFontMetrics->width(extentText);

            painter.setFont(m_normalFont);
            currentIndex = extent.end();
        }

        if (currentIndex < text.size()) {
            QString finalSection = text.right(text.size() - currentIndex);
            painter.drawText(currentX, baseline, finalSection);
        }

        currentLineOffset += lineHeight;
    }

    painter.end();
    painter.begin(this);
    painter.drawPixmap(QPoint(0,0), m_backBuffer);
    painter.end();

}

void SuggestionBox::searchBarChanged(const QString& string)
{
    if (!m_shouldStartNewSearchWhenLineEditChanges)
        return;

    if (m_searchTask) {
        m_searchTask->stop();
        m_searchTask = 0;
    }

    if (string.isNull() || string.isEmpty()) {
        hide();
        return;
    }

    m_originalSearchBarText = string;
    m_searchTask = new SearchTask(string, this);
    PeceraApplication::instance()->performFilenameSearch(m_searchTask);
    m_paintTimer.start(200);
}

int SuggestionBox::getLineHeight()
{
    return m_normalFontMetrics->height() + (SUGGESTION_LINE_PADDING * 2);
}

void SuggestionBox::returned()
{
    hide();

    if (m_searchTask) {
        m_searchTask->stop();
        m_searchTask = 0;
    }

    m_activeIndex = 0;
    if (m_activeResult)
        m_activeResult->executeAction();
}

void SuggestionBox::hide()
{
    QFrame::hide();
    m_activeIndex = 0;
}

}
