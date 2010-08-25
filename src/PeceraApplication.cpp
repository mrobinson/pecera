// -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include "PeceraApplication.h"
#include "Project.h"
#include <iostream>

#include <QVariant>

#include <QApplication>
#include <QGroupBox>
#include <QLineEdit>
#include <QObject>
#include <QProcess>
#include <QShortcut>
#include <QTabWidget>
#include <QThreadPool>
#include <QVBoxLayout>
#include <QX11EmbedContainer>
#include <QX11Info>
#include "SearchBar.h"
#include "SuggestionBox.h"
#include <X11/Xlib.h>

#include <QDesktopServices>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

namespace Pecera {

static PeceraApplication* g_application = 0;
PeceraApplication::PeceraApplication(int& argc, char** argv)
    : QApplication(argc, argv)
    , m_locationBarShortcut(0)
{
    g_application = this;

    this->setApplicationName(QString("pecera"));
        
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
            QApplication::exit(EXIT_FAILURE);
        }

        if(!query.exec("SELECT * FROM projects")) {
            std::cout << query.lastError().text().toStdString() 
                  << std::endl;
            QApplication::exit(EXIT_FAILURE);
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
            exit(EXIT_FAILURE);
        }
    }

    QThreadPool::globalInstance()->start(new LoadProjectTask(m_projects.at(0)));

    m_window = new QGroupBox();
    QVBoxLayout layout;

    m_windowLayout = new QVBoxLayout();
    m_searchBar = new SearchBar(m_projects.at(0));
    m_windowLayout->addWidget(m_searchBar);
    m_tabs = new QTabWidget();
    m_windowLayout->addWidget(m_tabs);

    m_window->setLayout(m_windowLayout);
    m_window->show();
    m_window->resize(800, 600);

    m_searchBar->tabs = m_tabs;
    m_searchBar->groupBox = m_window;

    reloadLocationShortcut();

}

PeceraApplication::~PeceraApplication()
{
    m_window->close();

    QList<Project*>::const_iterator i = m_projects.begin();
    while (i != m_projects.end())
        delete *i;

    delete m_tabs;
    delete m_searchBar;
    delete m_windowLayout;
    delete m_window;
}

PeceraApplication& PeceraApplication::getApplication()
{
    return *g_application;
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
    /*
  Display *display = QX11Info::display();

  if(event->type == KeyPress || event->type == KeyRelease) {
    //std::cout << event->xkey.keycode << std::endl;
  }
  if (event->type == ClientMessage) {
    std::cout << "ClientMessage" << std::endl;
    Atom atom = XInternAtom(display, "_XEMBED", FALSE);
    if(event->xclient.message_type == atom) {
      std::cout << "XEMBED" << std::endl;
    }
  }
    */
  QApplication::x11EventFilter(event);
}

}
