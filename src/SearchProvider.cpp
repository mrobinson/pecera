#include <QFile>
#include <QList>
#include <QMutexLocker>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QCoreApplication>
#include <QThreadPool>

#include "Result.h"
#include "SearchProvider.h"

namespace Pecera
{

SearchTask::SearchTask(const QString& query, SearchSubscriber* subscriber)
    : m_provider(0)
    , m_subscriber(subscriber)
    , m_query(query)
    , m_running(true)
{
}

void SearchTask::run()
{
    m_provider->performSearch(this);
}

bool SearchTask::newSearchResult(Result* result)
{
    if (!running())
        return false;

    m_running = m_subscriber->newSearchResult(result);
    return running();
}

bool SearchSubscriber::newSearchResult(Result* result)
{
    QMutexLocker lock(&m_resultsMutex);
    m_results.append(result);
    return m_results.size() <= 10;
}

void SearchProvider::scheduleSearch(SearchTask* task)
{
    // XXX: Don't use the thread pool here.
    task->setProvider(this);
    QThreadPool::globalInstance()->start(task);
}

NaiveSearchProvider::NaiveSearchProvider(const QString& namesFile)
{
    QFile file(namesFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd())
        m_filenames << in.readLine();
}

void NaiveSearchProvider::performSearch(SearchTask* task)
{
    Result* nextResult = new Result;
    QStringList parts(task->query().split(" ", QString::SkipEmptyParts));
    int numberOfParts = parts.size();
    int numberOfResults = 0;

    for (int i = 0; i < m_filenames.size(); ++i) {
        const QString& filename = m_filenames[i];
        int j = 0;
        for (; j < numberOfParts; ++j) {
            int index = filename.indexOf(parts[j], 0, Qt::CaseInsensitive);

            if (index == -1) {
                nextResult->extents().clear();
                break;
            } else {
                Extent extent(index, parts[j].size());
                nextResult->addExtent(extent);
            }
        }

        if (j == numberOfParts) {
            nextResult->setText(filename);
            task->newSearchResult(nextResult);
            nextResult = new Result;
            numberOfResults++;
        }

        if (!task->running()) {
            break;
        }
    }

    delete nextResult;
}

}
