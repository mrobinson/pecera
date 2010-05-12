#ifndef SearchProvider_h
#define SearchProvider_h

#include <QEvent>
#include <QMutex>
#include <QRunnable>

namespace Pecera
{
class SearchTask;
class SearchSubscriber;
class SearchProvider;
class Result;

class SearchTask : public QRunnable
{
public:
    SearchTask(const QString& query, SearchSubscriber* subscriber);
    void run();

    SearchProvider* provider() { return m_provider; }
    void setProvider(SearchProvider* provider) { m_provider = provider; }
    const QString& query() { return m_query; }
    void setQuery(const QString& query) { m_query = query; }
    bool running() { return m_running; }
    void stop() { m_running = false; }
    bool newSearchResult(Result* result);

private:
    SearchProvider* m_provider;
    SearchSubscriber* m_subscriber;
    QString m_query;
    bool m_running;
};

class SearchSubscriber
{
public:
    virtual ~SearchSubscriber() {};
    virtual bool newSearchResult(Result* result);

protected:
    QList<Result*> m_results;
    QMutex m_resultsMutex;
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
    NaiveSearchProvider(const QString& namesFile);
    ~NaiveSearchProvider() {}
    void performSearch(SearchTask* task);

private:
    QStringList m_filenames;
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
