#ifndef PeceraApplication_h
#define PeceraApplication_h

#include <QApplication>
#include <QObject>

class QShortcut;
class QGroupBox;
class QVBoxLayout;
class QTabWidget;

namespace Pecera {

class SearchBar;
class Project;

class PeceraApplication : public QApplication {
    Q_OBJECT

public:
    static PeceraApplication* instance();
    PeceraApplication(int& argc, char** argv);
    ~PeceraApplication();
    int exec();
    void reloadLocationShortcut();

    QString getGlobalStorageLocation();
    QString getProjectStorageLocation();

public slots:
    void focusSearchBar();

private:
    QShortcut* m_locationBarShortcut;
    QGroupBox* m_window;
    QVBoxLayout* m_windowLayout;
    QTabWidget* m_tabs;
    SearchBar* m_searchBar;
    QList<Project*> m_projects;

    bool x11EventFilter(XEvent* event);
};

}

#endif
