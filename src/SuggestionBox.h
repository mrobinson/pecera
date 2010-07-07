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
    SuggestionBox(SearchBar* bar);
    ~SuggestionBox();
    void setProject(Project* project) { m_project = project; }
    virtual bool newSearchResult(SearchTask*);
    virtual void searchComplete(SearchTask*);
    virtual bool eventFilter(QObject*, QEvent*);

signals:
    void forcePaint();

public slots:
    void lineEditChanged(const QString& string);
    void paintTimeout();

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
    QTimer m_paintTimer;
    QPixmap m_backBuffer;

    int getLineHeight();
};

}

#endif
