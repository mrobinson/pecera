#include "Project.h"
#include "Result.h"
#include "SearchProvider.h"
#include <QFile>
#include <QList>
#include <QMutexLocker>
#include <QPair>
#include <QString>
#include <QStringList>
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
    // XXX: Don't use the thread pool here.
    task->setProvider(this);
    QThreadPool::globalInstance()->start(task);
}

NaiveSearchProvider::NaiveSearchProvider(Project* project)
    : m_project(project)
{
}

void NaiveSearchProvider::performSearch(SearchTask* task)
{
    Result* nextResult = new Result;
    QStringList parts(task->query().split(" ", QString::SkipEmptyParts));
    int numberOfParts = parts.size();
    int numberOfResults = 0;

    const QHash<QString, File*> files = m_project->files();
    QHash<QString, File*>::const_iterator i;
    for (i = files.constBegin(); i != files.constEnd(); ++i) {
        const QString& filename = i.key();
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
    task->searchComplete();
}

}
