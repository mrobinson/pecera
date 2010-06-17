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
    virtual bool newSearchResult();
    void setProject(Project* project) { m_project = project; }

signals:
    void searchUpdated();

public slots:
    void lineEditChanged(const QString& string);
    void handleSearchUpdated();

protected:
    void paintEvent(QPaintEvent*);

private:
    SearchBar* m_bar;
    SearchTask* m_searchTask;
    QFont m_normalFont;
    QFont m_boldFont;
    QFontMetrics* m_normalFontMetrics;
    QFontMetrics* m_boldFontMetrics;
    Project* m_project;

    int getLineHeight();
};

}

#endif
