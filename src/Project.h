#ifndef Pecera_Project_h
#define Pecera_Project_h

#include <QDir>
#include <QHash>
#include <QFile>
#include <QFileInfo>
#include <QMutex>
#include <QString>

namespace Pecera
{

class Project;

class File
{
public:
    File(Project* project, const QString& relativePath, qint64 lastUpdated);
    File() {};
    ~File();
    QString projectURL();
    QString relativePath() const { return m_relativePath; } 
    QString absolutePath();
    QFileInfo* info();

    void setLastUpdated(qint64 lastUpdated) { m_lastUpdated = lastUpdated; }
    qint64 lastUpdated() const { return m_lastUpdated; }

private:
    Project* m_project;
    QString m_relativePath;
    QFileInfo* m_info;
    qint64 m_lastUpdated;
};

class Project
{
public:
    Project(const QString& name, const QDir& root);
    QString name() { return m_name; }
    QDir& root() { return m_root; }

    void addOrUpdateEntry(const QString&, qint64);
    QString getAbsolutePath(const QString&);
    QString getRelativePath(const QString&);

private:
    QString m_name;
    QDir m_root;
    QHash<QString, File> m_files;
    QMutex m_filesMutex;

    void scanRoot();
    void save();
    void load();
};

}
#endif
