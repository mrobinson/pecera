#include "Project.h"
#include "Result.h"
#include "SearchBar.h"
#include "SearchProvider.h"
#include "SuggestionBox.h"
#include <QPainter>
#include <QPaintEvent>

namespace Pecera
{

SuggestionBox::SuggestionBox(SearchBar* bar, Qt::WindowFlags flags)
    : QFrame(bar, flags)
    , m_bar(bar)
    , m_searchTask(0)
    , m_normalFont(font())
    , m_boldFont(font())
    , m_normalFontMetrics(0)
    , m_boldFontMetrics(0)
    , m_project(0)
    , m_paintTimer(this)
{
    m_boldFont.setBold(true);
    m_normalFontMetrics = new QFontMetrics(m_normalFont);
    m_boldFontMetrics = new QFontMetrics(m_boldFont);

    connect(this, SIGNAL(forcePaint()),
        this, SLOT(paintTimeout()));
    connect(&m_paintTimer, SIGNAL(timeout()),
        this, SLOT(paintTimeout()));
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
    const QRect& geometry = m_bar->geometry();
    const QPoint& point = m_bar->mapToGlobal(QPoint(0, geometry.height()));
    if (!isVisible()) {
        QMutexLocker lock(m_searchTask->resultsMutex());
        this->setGeometry(point.x(), point.y(), geometry.width(), getLineHeight() * (m_searchTask->results().size() + 1));
        show();
    } else {
        {
            QMutexLocker lock(m_searchTask->resultsMutex());
            this->resize(geometry.width(), getLineHeight() * (m_searchTask->results().size() + 1));
        }
    }
    repaint(rect());
}

void SuggestionBox::paintEvent(QPaintEvent* event)
{
    if (isHidden())
        return;

    QPainter painter(this);
    painter.fillRect(event->rect(), QBrush(Qt::white));

    int lineHeight = getLineHeight();
    int baseline = SUGGESTION_LINE_PADDING + m_normalFontMetrics->height();

    painter.drawText(0, baseline, "Full text search...");
    baseline += lineHeight;

    QMutexLocker lock(m_searchTask->resultsMutex());
    for (int i = 0; i < m_searchTask->results().size(); i++) {
        Result* result = m_searchTask->results()[i];
        const QString& text = result->text();

        int currentIndex = 0;
        int currentX = 0;
        const QList<Pecera::Extent>& extents = result->extents();
        for (int j = 0; j < extents.size(); ++j) {
            const Pecera::Extent& extent = extents[j];

            QString beforeExtent = text.mid(currentIndex, extent.start() - currentIndex);
            if (beforeExtent.size() > 0) {
                painter.drawText(currentX, baseline, beforeExtent);
                currentX += m_normalFontMetrics->tightBoundingRect(beforeExtent).width();
            }

            QString extentText = text.mid(extent.start(), extent.length());
            painter.setFont(m_boldFont);
            painter.drawText(currentX, baseline, extentText);
            currentX += m_boldFontMetrics->tightBoundingRect(extentText).width();

            painter.setFont(m_normalFont);
            currentIndex = extent.end();
        }

        if (currentIndex < text.size()) {
            QString finalSection = text.right(text.size() - currentIndex);
            painter.drawText(currentX, baseline, finalSection);
        }

        baseline += lineHeight;
    }
}

void SuggestionBox::lineEditChanged(const QString& string)
{
    if (m_searchTask)
        m_searchTask->stop();

    if (string.isNull() || string.isEmpty()) {
        hide();
        return;
    }

    m_searchTask = new SearchTask(string, this);
    m_project->performFilenameSearch(m_searchTask);
    m_paintTimer.start(200);
}

int SuggestionBox::getLineHeight()
{
    return m_normalFontMetrics->height() + (SUGGESTION_LINE_PADDING * 2);
}

}
