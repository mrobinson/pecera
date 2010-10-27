#include "FilenameSearchProvider.h"
#include "PeceraApplication.h"
#include "Project.h"
#include "Window.h"
#include <assert.h>
#include <QWidget>
#include <QGroupBox>
#include <QHash>
#include <QStringList>
#include <QGridLayout>

namespace Pecera
{

FilenameSearchProvider::FilenameSearchProvider()
{
}

static void performSearchOfProject(SearchTask* task, Project* project)
{
    FilenameSearchResult* nextResult = new FilenameSearchResult(project);
    QStringList parts(task->query().split(" ", QString::SkipEmptyParts));
    int numberOfParts = parts.size();
    int numberOfResults = 0;

    const QHash<QString, File*> files = project->files();
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
            nextResult->setFile(i.value());
            task->newSearchResult(nextResult);
            nextResult = new FilenameSearchResult(project);
            numberOfResults++;
        }

        if (!task->running()) {
            break;
        }
    }

    delete nextResult;
    task->searchComplete();
}

void FilenameSearchProvider::performSearch(SearchTask* task)
{
    const QList<Project*>& projects = PeceraApplication::instance()->projects();
    QList<Project*>::const_iterator i;
    for (i = projects.constBegin(); i != projects.constEnd(); i++) {
        performSearchOfProject(task, *i);
        if (!task->running())
            return;
    }
}

FilenameSearchResult::FilenameSearchResult(Project* project)
    : m_project(project)
{
}

void FilenameSearchResult::executeAction()
{
    QFile targetFile(m_project->getAbsolutePath(text()));
    if (!targetFile.exists())
        return;

    PeceraApplication* app = PeceraApplication::instance();
    app->window()->tabs()->setTab(&targetFile);
}

void FilenameSearchResult::setFile(File* file)
{
    assert(file);
    m_url = file->projectURL();
    m_text = file->relativePath();
}

}
