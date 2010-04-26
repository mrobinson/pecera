#include <QPainter>
#include <QPaintEvent>
#include "Result.h"
#include "SearchBar.h"
#include "SearchProvider.h"
#include "SuggestionBox.h"

namespace Pecera
{

SuggestionBox::SuggestionBox(SearchBar* bar, Qt::WindowFlags flags)
    : QFrame(bar, flags)
    , m_bar(bar)
    , m_searchProvider(new NaiveSearchProvider("fnames.txt"))
    , m_searchTask(0)
    , m_normalFont(font())
    , m_boldFont(font())
    , m_normalFontMetrics(0)
    , m_boldFontMetrics(0)
{
    m_boldFont.setBold(true);
    m_normalFontMetrics = new QFontMetrics(m_normalFont);
    m_boldFontMetrics = new QFontMetrics(m_boldFont);

    connect(this, SIGNAL(searchUpdated()),
        this, SLOT(handleSearchUpdated()));
}

SuggestionBox::~SuggestionBox()
{
    if (m_boldFontMetrics)
        delete m_boldFontMetrics;
    if (m_normalFontMetrics)
        delete m_normalFontMetrics;
}

bool SuggestionBox::newSearchResult(Result* result)
{
    bool keepGoing = SearchSubscriber::newSearchResult(result);

    const QRect& geometry = m_bar->geometry();
    const QPoint& point = m_bar->mapToGlobal(m_bar->pos());
    if (!isVisible()) {
        this->setGeometry(
            point.x(), point.y() + geometry.height(),
            geometry.width(), getLineHeight() * m_results.size());
        show();
    } else {
        this->resize(geometry.width(), getLineHeight() * m_results.size());
    }

    return keepGoing;
}

void SuggestionBox::handleSearchUpdated()
{
    repaint(rect());
    show();
}

void SuggestionBox::paintEvent(QPaintEvent* event)
{
    if (isHidden())
        return;

    QPainter painter(this);
    painter.fillRect(event->rect(), QBrush(Qt::white));

    int lineHeight = getLineHeight();
    int baseline = SUGGESTION_LINE_PADDING + m_normalFontMetrics->height();

    for (int i = 0; i < m_results.size(); i++) {
        Result* result = m_results[i];
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
    printf("Line edit changed: %s\n", string.toUtf8().data());
    if (m_searchTask)
        m_searchTask->stop();
    m_results.clear();

    if (string.isNull() || string.isEmpty()) {
        hide();
        return;
    }

    m_searchTask = new SearchTask(string, this);
    Result* result = new Result;
    static QString fullText("Full text search for: ");
    result->setText(fullText + string);
    newSearchResult(result);
    m_searchProvider->performSearch(m_searchTask);
}

int SuggestionBox::getLineHeight()
{
    return m_normalFontMetrics->height() + (SUGGESTION_LINE_PADDING * 2);
}

}
