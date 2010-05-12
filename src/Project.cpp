#include "Project.h"
#include <QDateTime>
#include <QDesktopServices>
#include <QMutexLocker>
#include <QStack>
#include <sqlite3.h>

namespace Pecera
{

File::File(Project* project, const QString& relativePath, qint64 lastUpdated)
    : m_project(project)
    , m_relativePath(relativePath)
    , m_info(0)
    , m_lastUpdated(lastUpdated)
{
}

File::~File()
{
    if (m_info)
        delete m_info;
}

QString File::projectURL()
{
    QString projectURL("proj://");
    projectURL += m_project->name();
    projectURL += "/";
    projectURL += m_relativePath;
    return projectURL;
}

QFileInfo* File::info()
{
    if (!m_info)
        m_info = new QFileInfo(m_project->getAbsolutePath(m_relativePath));
    return m_info;
}

QString File::absolutePath()
{
    return m_project->getRelativePath(m_relativePath);
}

Project::Project(const QString& name, const QDir& root)
    : m_name(name)
    , m_root(root)
{
}

QString Project::getRelativePath(const QString& absolutePath)
{
    return m_root.relativeFilePath(absolutePath);
}

QString Project::getAbsolutePath(const QString& relativePath)
{
    return m_root.absoluteFilePath(relativePath);
}

void Project::addOrUpdateEntry(const QString& relativePath, qint64 lastUpdated)
{
    QMutexLocker lock(&m_filesMutex);
    if (m_files.contains(relativePath)) {
        m_files[relativePath].setLastUpdated(lastUpdated);
        return;
    }

    m_files[relativePath] = File(this, relativePath, lastUpdated);
}

void Project::scanRoot()
{
    QStack<QDir> directoryStack;
    directoryStack.push(m_root);

    while (!directoryStack.empty()) {
        QDir directory(directoryStack.pop());
        qint64 updated = QDateTime::currentDateTime().toTime_t();
        QFileInfoList entries(directory.entryInfoList(QDir::NoFilter, QDir::DirsLast));

        for (int i = 0; i < entries.length(); i++) {
            QString absolutePath(entries[i].absoluteFilePath());
            addOrUpdateEntry(getRelativePath(absolutePath), updated);

            if (entries[i].isDir())
                directoryStack.push(QDir(absolutePath));
        }
    }
}

static int fileReadFromStorageCallback(void* data, int columnCount, char** results, char**)
{
    Project* project = static_cast<Project*>(data);
    if (columnCount < 2)
        return 1;

    project->addOrUpdateEntry(QString::fromUtf8(results[0]),
         QString(results[1]).toLongLong());
    return 0;
}

void Project::load()
{
    QDir storageLocation(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    if (!storageLocation.exists())
        return;

    QFile projectFile(storageLocation.filePath(m_name + ".db"));
    if (!projectFile.exists())
        return;

    // FIXME: Better error reporting
    // From: http://www.sqlite.org/c3ref/open.html -- The encoding used for
    // the filename argument of sqlite3_open() and  sqlite3_open_v2() must be
    // UTF-8, not whatever codepage is currently defined.
    sqlite3* database;
    int rc = sqlite3_open(projectFile.fileName().toUtf8().data(), &database);
    if (rc)
        return;

    char* errorMessage;
    rc = sqlite3_exec(database, "SELECT * from files", fileReadFromStorageCallback, this, &errorMessage);
    if (rc)
        sqlite3_free(errorMessage);

    sqlite3_close(database);
}

void Project::save()
{
    QDir storageLocation(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    if (!storageLocation.exists()) {
        if (!storageLocation.mkpath(storageLocation.absolutePath()))
            return;
    }

    QFile projectFile(storageLocation.filePath(m_name + ".db"));

    // From: http://www.sqlite.org/c3ref/open.html -- The encoding used for
    // the filename argument of sqlite3_open() and  sqlite3_open_v2() must be
    // UTF-8, not whatever codepage is currently defined.
    sqlite3* database;
    int rc = sqlite3_open(projectFile.fileName().toUtf8().data(), &database);
    if (rc)
        return;

    char* errorMessage;
    rc = sqlite3_exec(database, "CREATE TALBE IF NOT EXISTS files(path TEXT, last_updated INT);", 0, 0, &errorMessage);
    if (rc)
        sqlite3_free(errorMessage);

    sqlite3_stmt* statement;
    const char* sql = "INSERT OR REPLACE INTO files(path, last_updated) values(?);";
    const char* tail;
    if (sqlite3_prepare(database, sql, strlen(sql), &statement, &tail) != SQLITE_OK)
        return;

    QMutexLocker lock(&m_filesMutex);
    QHash<QString, File>::const_iterator i = m_files.begin();
    while (i != m_files.end()) {
        const File& file = i.value();
        QByteArray path(file.relativePath().toUtf8());
        sqlite3_bind_text(statement, 0, path, path.size(), SQLITE_STATIC);
        sqlite3_bind_int64(statement, 1, file.lastUpdated());

        // BIG FIXME: Check the return value here and do the right thing.
        sqlite3_step(statement);
        sqlite3_reset(statement);

        i++;
    }

    sqlite3_finalize(statement);
    sqlite3_close(database);
}

}
