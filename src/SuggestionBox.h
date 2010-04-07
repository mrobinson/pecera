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
    int getLineHeight();
};

}

#endif
