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
    printf("subscriber\n");
    bool keepGoing = SearchSubscriber::newSearchResult(result);
    printf("--------\n");
    for (int i = 0; i < m_results.size(); i++)
    {
        printf("%s\n", m_results[i]->text().toUtf8().data());
    }
    //emit searchUpdated;
    this->setGeometry(
        m_bar->geometry().x(),
        m_bar->geometry().y() + m_bar->geometry().height(),
        m_bar->geometry().width(),
        getLineHeight() * m_results.size());
    printf("%i %i %i\n", getLineHeight(), m_results.size(), getLineHeight() * m_results.size());
    repaint(rect());
    show();
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
    int fontHeight = painter.fontMetrics().height();
    printf("results size:%i\n", m_results.size());
    for (int i = 0; i < m_results.size(); i++) {
        int baseline = (i * lineHeight) + SUGGESTION_LINE_PADDING + fontHeight;
        painter.drawText(SUGGESTION_LINE_PADDING, baseline,
            m_results[i]->text());
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
