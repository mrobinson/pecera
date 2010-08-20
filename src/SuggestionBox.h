#ifndef SuggestionBox_h
#define SuggestionBox_h

#include <QFrame>
#include <QTimer>
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
    SuggestionBox(SearchBar*, Project*);
    ~SuggestionBox();
    virtual bool newSearchResult(SearchTask*);
    virtual void searchComplete(SearchTask*);
    virtual bool eventFilter(QObject*, QEvent*);

signals:
    void forcePaint();

public slots:
    void searchBarChanged(const QString& string);
    void paintTimeout();

protected:
    void paintEvent(QPaintEvent*);

private:
    int getLineHeight();
    void setActiveIndex(int newActiveIndex);

    SearchBar* m_bar;
    SearchTask* m_searchTask;
    unsigned int m_activeIndex;

    // TODO: Find a better way to do this.
    bool m_shouldStartNewSearchWhenLineEditChanges;
    QFont m_normalFont;
    QFont m_boldFont;
    QFontMetrics* m_normalFontMetrics;
    QFontMetrics* m_boldFontMetrics;
    Project* m_project;
    QTimer m_paintTimer;
    QPixmap m_backBuffer;

};

}

#endif
