// -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include "FilenameSearchProvider.h"
#include "FullTextSearchProvider.h"
#include "PeceraApplication.h"
#include "Project.h"
#include "SearchBar.h"
#include "SuggestionBox.h"
#include <iostream>
#include <QApplication>
#include <QDesktopServices>
#include <QGroupBox>
#include <QLineEdit>
#include <QObject>
#include <QProcess>
#include <QShortcut>
#include <QTabWidget>
#include <QThreadPool>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QVariant>
#include <QVBoxLayout>

namespace Pecera {

PeceraApplication::PeceraApplication(int& argc, char** argv)
    : QApplication(argc, argv)
    , m_locationBarShortcut(0)
{
    this->setApplicationName(QString("pecera"));
}

PeceraApplication::~PeceraApplication()
{
    QList<Project*>::const_iterator i = m_projects.begin();
    while (i != m_projects.end())
        delete *i;

    if (m_window)
        m_window->close();
    delete m_tabs;
    delete m_searchBar;
    delete m_windowLayout;
    delete m_window;
}

int PeceraApplication::exec()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QDir projectDir = QDir(this->getGlobalStorageLocation());
    projectDir.mkpath(projectDir.absolutePath());
    QString dbPath = this->getGlobalStorageLocation().append(QString("/pecera.db"));
    db.setDatabaseName(dbPath);

    if(db.open()) {
        QSqlQuery query(db);
        if(!query.exec(QString("CREATE TABLE IF NOT EXISTS projects (name TEST, description TEXT, path TEXT, CONSTRAINT projects_pkey PRIMARY KEY (name))"))) {
            std::cout << query.lastError().text().toStdString() 
                  << std::endl;
            return EXIT_FAILURE;
        }

        if(!query.exec("SELECT * FROM projects")) {
            std::cout << query.lastError().text().toStdString() 
                  << std::endl;
            return EXIT_FAILURE;
        }

        int projectNameIndex = query.record().indexOf("name");
        int projectPathIndex = query.record().indexOf("path");
        while (query.next()) {
            std::cout << query.value(projectPathIndex).toString().toStdString()
                  << std::endl;
            m_projects.append(new Project(query.value(projectNameIndex).toString(), QDir(query.value(projectPathIndex).toString())));
            break;
        }
        db.close();

        if(this->m_projects.size() == 0) {
            std::cerr << "sqlite3 " << dbPath.toStdString()
                  << std::endl
                  << " INSERT INTO projects (name, description, path) VALUES('yourprojectname', 'yourdescription', 'yourpath')"
                  << std::endl;
            return EXIT_FAILURE;
        }
    }

    QThreadPool::globalInstance()->start(new LoadProjectTask(m_projects.at(0)));

    m_window = new QGroupBox();
    QVBoxLayout layout;

    m_windowLayout = new QVBoxLayout();
    m_searchBar = new SearchBar();
    m_windowLayout->addWidget(m_searchBar);
    m_tabs = new QTabWidget();
    m_windowLayout->addWidget(m_tabs);

    m_window->setLayout(m_windowLayout);
    m_window->show();
    m_window->resize(800, 600);

    reloadLocationShortcut();

    return QApplication::exec();
}

PeceraApplication* PeceraApplication::instance()
{
    return static_cast<PeceraApplication*>(QApplication::instance());
}

void PeceraApplication::reloadLocationShortcut()
{
    if (m_locationBarShortcut) {
        delete m_locationBarShortcut;
        m_locationBarShortcut = 0;
    }

    this->m_locationBarShortcut = new QShortcut(
        QKeySequence(QObject::tr("Ctrl+L", "File|Open")), m_window);
    QObject::connect(this->m_locationBarShortcut, SIGNAL(activated()), 
        this, SLOT(focusSearchBar()));
}


QString PeceraApplication::getGlobalStorageLocation() {
    return QDesktopServices::storageLocation
        (QDesktopServices::DataLocation);
}

QString PeceraApplication::getProjectStorageLocation() {
    return QDesktopServices::storageLocation
        (QDesktopServices::DataLocation).append(QString("/projects"));
}

void PeceraApplication::focusSearchBar()
{
    m_searchBar->selectAll();
    m_searchBar->setFocus();
}

bool PeceraApplication::x11EventFilter(XEvent* event) 
{
  QApplication::x11EventFilter(event);
}

void PeceraApplication::performFilenameSearch(SearchTask* task)
{
    task->newSearchResult(new FullTextSearchResult(task->query()));
    m_filenameSearchProvider.scheduleSearch(task);
}

}
