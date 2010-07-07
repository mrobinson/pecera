#ifndef SearchProvider_h
#define SearchProvider_h

#include <QEvent>
#include <QList>
#include <QMutex>
#include <QRunnable>
#include <QString>
#include <cstdio>


namespace Pecera
{
class SearchTask;
class SearchSubscriber;
class SearchProvider;
class Result;
class Project;

class SearchTask : public QRunnable
{
public:
    SearchTask(const QString& query, SearchSubscriber* subscriber);
    void run();

    SearchProvider* provider() { return m_provider; }
    void setProvider(SearchProvider* provider) { m_provider = provider; }
    const QString& query() { return m_query; }
    void setQuery(const QString& query) { m_query = query; }

    QList<Result*>& results() { return m_results; }
    QMutex* resultsMutex() { return &m_resultsMutex; }
    bool newSearchResult(Result* result);
    void searchComplete();

    bool running() { return m_running; }
    void stop() { m_running = false; }

private:
    SearchProvider* m_provider;
    SearchSubscriber* m_subscriber;
    QString m_query;
    QList<Result*> m_results;
    QMutex m_resultsMutex;
    bool m_running;
};

class SearchSubscriber
{
public:
    virtual ~SearchSubscriber() {};
    virtual bool newSearchResult(SearchTask*) = 0;
    virtual void searchComplete(SearchTask*) = 0;
};

class SearchProvider
{
public:
    virtual ~SearchProvider() {}
    void scheduleSearch(SearchTask* task);
    virtual void performSearch(SearchTask* task) = 0;
}; 

class NaiveSearchProvider : public SearchProvider
{
public:
    NaiveSearchProvider(Project* project);
    ~NaiveSearchProvider() {}
    void performSearch(SearchTask* task);

private:
    Project* m_project;
};

class SearchUpdatedEvent : public QEvent
{
public:
    SearchUpdatedEvent(const QList<Result*>& results);
    static QEvent::Type eventType();
    QList<Result*> results() { return m_results; }

private:
    QList<Result*> m_results;
};

}

#endif
