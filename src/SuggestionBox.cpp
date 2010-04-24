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
{
    connect(this, SIGNAL(searchUpdated()),
        this, SLOT(handleSearchUpdated()));
}

bool SuggestionBox::newSearchResult(Result* result)
{
    bool keepGoing = SearchSubscriber::newSearchResult(result);

    if (m_results.size() < 5)
        return true;

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
    const QFontMetrics& metrics = painter.fontMetrics();
    int baseline = SUGGESTION_LINE_PADDING + metrics.height();

    QFont normalFont(painter.font());
    QFont boldFont(painter.font());
    boldFont.setBold(true);

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
                currentX += metrics.boundingRect(beforeExtent).width();
            }

            QString extentText = text.mid(extent.start(), extent.length());
            painter.setFont(boldFont);
            painter.drawText(currentX, baseline, extentText);
            currentX += metrics.boundingRect(extentText).width();

            painter.setFont(normalFont);
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
    static int lineHeight = -1;
    if (lineHeight == -1)
        lineHeight = fontMetrics().height() + (SUGGESTION_LINE_PADDING * 2);

    return lineHeight;
}

}
