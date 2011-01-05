#ifndef PeceraApplication_h
#define PeceraApplication_h

#include "FilenameSearchProvider.h"
#include "CtagsSearchProvider.h"
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
class Window;

class PeceraApplication : public QApplication {
    Q_OBJECT

public:
    static PeceraApplication* instance();
    PeceraApplication(int& argc, char** argv);
    ~PeceraApplication();

    const QList<Project*>& projects() { return m_projects; }
    Window* window() { return m_window; }

    int exec();
    int initApplicationDatabase();
    int createProject(QString title, QString path);
    void reloadLocationShortcut();
    QString getGlobalStorageLocation();
    QString getProjectStorageLocation();
    void performFilenameSearch(SearchTask*);

private:
    QShortcut* m_locationBarShortcut;
    Window* m_window;
    QList<Project*> m_projects;
    FilenameSearchProvider m_filenameSearchProvider;
    CtagsSearchProvider m_ctagsSearchProvider;

    void readProjects();
    bool x11EventFilter(XEvent* event);

private slots:
    void handleLastWindowClosed();
};

}

#endif
