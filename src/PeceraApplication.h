#ifndef PeceraApplication_h
#define PeceraApplication_h

#include "FilenameSearchProvider.h"
#include <QApplication>
#include <QObject>

class QShortcut;
class QGroupBox;
class QVBoxLayout;
class QTabWidget;

namespace Pecera {

class Project;
class SearchBar;
class SearchTask;

class PeceraApplication : public QApplication {
    Q_OBJECT

public:
    static PeceraApplication* instance();
    PeceraApplication(int& argc, char** argv);
    ~PeceraApplication();

    const QList<Project*>& projects() { return m_projects; }
    QGroupBox* window() { return m_window; }
    QTabWidget* tabs() { return m_tabs; }

    int exec();
    void reloadLocationShortcut();
    QString getGlobalStorageLocation();
    QString getProjectStorageLocation();
    void performFilenameSearch(SearchTask*);

public slots:
    void focusSearchBar();

private:
    QShortcut* m_locationBarShortcut;
    QGroupBox* m_window;
    QVBoxLayout* m_windowLayout;
    QTabWidget* m_tabs;
    SearchBar* m_searchBar;
    QList<Project*> m_projects;
    FilenameSearchProvider m_filenameSearchProvider;

    bool x11EventFilter(XEvent* event);
};

}

#endif
