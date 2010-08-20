#include "PeceraApplication.h"
#include "Project.h"
#include <iostream>
#include <QApplication>
#include <QGroupBox>
#include <QLineEdit>
#include <QObject>
#include <QProcess>
#include <QShortcut>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QX11EmbedContainer>
#include <QX11Info>
#include "SearchBar.h"
#include "SuggestionBox.h"
#include <X11/Xlib.h>

namespace Pecera {

static PeceraApplication* g_application = 0;
PeceraApplication::PeceraApplication(int& argc, char** argv)
    : QApplication(argc, argv)
    , m_locationBarShortcut(0)
{
    g_application = this;

    // FIXME: Need to get this data from configuration soon.
    m_project = new Project(QString("slideshow"), QDir("/home/martin/app/titanium_desktop"));
    m_project->scanRoot(); 
    m_project->save(); 

    m_window = new QGroupBox();
    QVBoxLayout layout;

    m_windowLayout = new QVBoxLayout();
    m_searchBar = new SearchBar(m_project);
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
    delete m_project;
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

void PeceraApplication::focusSearchBar()
{
    m_searchBar->selectAll();
    m_searchBar->setFocus();
}

bool PeceraApplication::x11EventFilter(XEvent* event) {
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

  QApplication::x11EventFilter(event);
}

}
