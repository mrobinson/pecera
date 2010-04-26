#ifndef SuggestionBox_h
#define SuggestionBox_h

#include <QFrame>
#include "SearchProvider.h"

class SearchBar;
namespace Pecera
{
class Result;
class SearchUpdatedEvent;

class SuggestionBox : public QFrame, public SearchSubscriber
{
    Q_OBJECT

public:
    SuggestionBox(SearchBar* bar, Qt::WindowFlags flags);
    ~SuggestionBox();
    void suggestionsUpdated(SearchUpdatedEvent* event);
    bool newSearchResult(Result* result);

signals:
    void searchUpdated();

public slots:
    void lineEditChanged(const QString& string);
    void handleSearchUpdated();

protected:
    void paintEvent(QPaintEvent*);

private:
    SearchBar* m_bar;
    SearchProvider* m_searchProvider;
    SearchTask* m_searchTask;
    QFont m_normalFont;
    QFont m_boldFont;
    QFontMetrics* m_normalFontMetrics;
    QFontMetrics* m_boldFontMetrics;

    int getLineHeight();
};

}

#endif
