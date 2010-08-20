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
    static PeceraApplication& getApplication();
    PeceraApplication(int& argc, char** argv);
    ~PeceraApplication();
    void reloadLocationShortcut();

public slots:
    void focusSearchBar();

private:
    QShortcut* m_locationBarShortcut;
    QGroupBox* m_window;
    QVBoxLayout* m_windowLayout;
    QTabWidget* m_tabs;
    SearchBar* m_searchBar;

    // FIXME: temporary hack until we support list of current projects
    Project* m_project;

    bool x11EventFilter(XEvent* event);
};

}

#endif
