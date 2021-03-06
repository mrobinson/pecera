#ifndef Pecera_Project_h
#define Pecera_Project_h

#include "SearchProvider.h"
#include <QDir>
#include <QHash>
#include <QFile>
#include <QFileInfo>
#include <QMutex>
#include <QString>

namespace Pecera
{

class Project;
class SearchTask;

class File
{
public:
    File(Project* project, const QString& relativePath, qint64 lastUpdated);
    File() {};
    ~File();
    QString projectURL() const;
    QString relativePath() const { return m_relativePath; } 
    QString absolutePath();
    const QFileInfo& info();

    void setLastUpdated(qint64 lastUpdated) { m_lastUpdated = lastUpdated; }
    qint64 lastUpdated() const { return m_lastUpdated; }

private:
    Project* m_project;
    QString m_relativePath;
    QFileInfo m_info;
    qint64 m_lastUpdated;
};

class Project
{
public:
    Project(const QString& name, const QDir& root);
    const QString name() { return m_name; }
    const QDir& root() { return m_root; }
    const QHash<QString, File*>& files() { return m_files; }

    void addOrUpdateEntry(const QString&, qint64);
    QString getAbsolutePath(const QString&);
    QString getRelativePath(const QString&);
    void scanRoot();
    void save();
    void load();
    void reportError(const QString&);

private:
    QString m_name;
    QDir m_root;
    QHash<QString, File*> m_files;
    QMutex m_filesMutex;
};

class LoadProjectTask : public QRunnable
{
public:
    LoadProjectTask(Project* project)
        : m_project(project)
    {
    }

    void run();

private:
    Project* m_project;
};

}
#endif
