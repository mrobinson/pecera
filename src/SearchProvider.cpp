#include "Result.h"
#include "SearchProvider.h"
#include <QFile>
#include <QList>
#include <QMutexLocker>
#include <QPair>
#include <QString>
#include <QTextStream>
#include <QCoreApplication>
#include <QThreadPool>

namespace Pecera
{

SearchTask::SearchTask(const QString& query, SearchSubscriber* subscriber)
    : m_provider(0)
    , m_subscriber(subscriber)
    , m_query(query)
    , m_running(true)
{
    setAutoDelete(false);
}

void SearchTask::run()
{
    m_provider->performSearch(this);
}

bool SearchTask::newSearchResult(Result* result)
{
    if (!running())
        return false;

    {
        QMutexLocker lock(&m_resultsMutex);
        m_results.append(result);
    }
    m_running = m_subscriber->newSearchResult(this);
}

void SearchTask::searchComplete()
{
    m_subscriber->searchComplete(this);
}

void SearchProvider::scheduleSearch(SearchTask* task)
{
    task->setProvider(this);
    QThreadPool::globalInstance()->start(task);
}
}
