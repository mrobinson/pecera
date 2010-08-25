// -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include "Project.h"
#include <QDateTime>
#include <QDesktopServices>
#include <QMutexLocker>
#include <QStack>
#include <sqlite3.h>

#include "PeceraApplication.h"

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
    , m_filenameSearchProvider(this)
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
        m_files[relativePath]->setLastUpdated(lastUpdated);
        return;
    }

    m_files[relativePath] = new File(this, relativePath, lastUpdated);
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
            if (entries[i].fileName() == "." || entries[i].fileName() == "..")
                continue;

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
    QDir storageLocation(PeceraApplication::getApplication().getProjectStorageLocation());
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
    if (rc) {
        reportError(sqlite3_errmsg(database));
        sqlite3_close(database);
        return;
    }

    char* errorMessage;
    rc = sqlite3_exec(database, "SELECT * from files", fileReadFromStorageCallback, this, &errorMessage);
    if (rc)
        sqlite3_free(errorMessage);

    sqlite3_close(database);
}

static void handleSQLiteError(Project* project, int rc, sqlite3* database, char* errorMessage = 0)
{
    QString error;
    switch (rc) {
        case SQLITE_ERROR: error = "SQLITE_ERROR"; break;
        case SQLITE_INTERNAL: error = "SQLITE_INTERNAL"; break;
        case SQLITE_PERM: error = "SQLITE_PERM"; break;
        case SQLITE_ABORT: error = "SQLITE_ABORT"; break;
        case SQLITE_BUSY: error = "SQLITE_BUSY"; break;
        case SQLITE_LOCKED: error = "SQLITE_LOCKED"; break;
        case SQLITE_NOMEM: error = "SQLITE_NOMEM"; break;
        case SQLITE_READONLY: error = "SQLITE_READONLY"; break;
        case SQLITE_INTERRUPT: error = "SQLITE_INTERRUPT"; break;
        case SQLITE_IOERR: error = "SQLITE_IOERR"; break;
        case SQLITE_CORRUPT: error = "SQLITE_CORRUPT"; break;
        case SQLITE_NOTFOUND: error = "SQLITE_NOTFOUND"; break;
        case SQLITE_FULL: error = "SQLITE_FULL"; break;
        case SQLITE_CANTOPEN: error = "SQLITE_CANTOPEN"; break;
        case SQLITE_PROTOCOL: error = "SQLITE_PROTOCOL"; break;
        case SQLITE_EMPTY: error = "SQLITE_EMPTY"; break;
        case SQLITE_SCHEMA: error = "SQLITE_SCHEMA"; break;
        case SQLITE_TOOBIG: error = "SQLITE_TOOBIG"; break;
        case SQLITE_CONSTRAINT: error = "SQLITE_CONSTRAINT"; break;
        case SQLITE_MISMATCH: error = "SQLITE_MISMATCH"; break;
        case SQLITE_MISUSE: error = "SQLITE_MISUSE"; break;
        case SQLITE_NOLFS: error = "SQLITE_NOLFS"; break;
        case SQLITE_AUTH: error = "SQLITE_AUTH"; break;
        case SQLITE_FORMAT: error = "SQLITE_FORMAT"; break;
        case SQLITE_RANGE: error = "SQLITE_RANGE"; break;
        case SQLITE_NOTADB: error = "SQLITE_NOTADB"; break;
        case SQLITE_ROW: error = "SQLITE_ROW"; break;
        case SQLITE_DONE: error = "SQLITE_DONE"; break;
    }
    
    error.append(": ");
    error.append(sqlite3_errmsg(database));

    if (errorMessage) {
        error.append(", ");
        error.append(errorMessage);
        sqlite3_free(errorMessage);
    }

    sqlite3_close(database);
    project->reportError(error);
}

void Project::save()
{
    QDir storageLocation(PeceraApplication::getApplication().getProjectStorageLocation());
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
    if (rc) {
        handleSQLiteError(this, rc, database);
        return;
    }

    char* errorMessage = 0;
    rc = sqlite3_exec(database, "BEGIN;", 0, 0, &errorMessage);
    if (rc) {
        handleSQLiteError(this, rc, database, errorMessage);
        return;
    }

    rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS files(path TEXT, last_updated INT);", 0, 0, &errorMessage);
    if (rc) {
        handleSQLiteError(this, rc, database, errorMessage);
        return;
    }

    rc = sqlite3_exec(database, "DELETE FROM files;", 0, 0, &errorMessage);
    if (rc) {
        handleSQLiteError(this, rc, database, errorMessage);
        return;
    }

    sqlite3_stmt* statement;
    const char* sql = "INSERT OR REPLACE INTO files(path, last_updated) values(?, ?);";
    const char* tail;
    rc = sqlite3_prepare_v2(database, sql, strlen(sql), &statement, &tail);
    if (rc != SQLITE_OK) {
        handleSQLiteError(this, rc, database);
        return;
    }

    QMutexLocker lock(&m_filesMutex);
    QHash<QString, File*>::const_iterator i = m_files.begin();
    while (i != m_files.end()) {
        File* file = i.value();
        char* path = strdup(file->relativePath().toUtf8().data());
        sqlite3_bind_text(statement, 1, path, strlen(path), SQLITE_STATIC);
        sqlite3_bind_int64(statement, 2, file->lastUpdated());

        // BIG FIXME: Check the return value here and do the right thing.
        rc = sqlite3_step(statement);
        if (rc != SQLITE_DONE) {
            handleSQLiteError(this, rc, database);
            return;
        }

        rc = sqlite3_reset(statement);
        if (rc != SQLITE_OK) {
            handleSQLiteError(this, rc, database);
            return;
        }

        i++;
    }

    rc = sqlite3_finalize(statement);
    if (rc != SQLITE_OK) {
        handleSQLiteError(this, rc, database);
        return;
    }

    rc = sqlite3_exec(database, "END;", 0, 0, &errorMessage);
    if (rc) {
        handleSQLiteError(this, rc, database, errorMessage);
        return;
    }

    sqlite3_close(database);
}

void Project::reportError(const QString& error)
{
    fprintf(stderr, "%s\n", error.toUtf8().data());
}

void Project::performFilenameSearch(SearchTask* task)
{
    m_filenameSearchProvider.scheduleSearch(task);
}

}
