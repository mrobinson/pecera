#ifndef Window_h
#define Window_h

#include <QGroupBox>
#include <QTabWidget>
#include <QVBoxLayout>

namespace Pecera
{
class SearchBar;

class Window : public QGroupBox
{
    Q_OBJECT

public:
    Window(QWidget* parent=0);
    ~Window();
    QTabWidget* tabs() { return &m_tabs; }

public slots:
    void focusSearchBar();

protected:
    void closeEvent(QCloseEvent*);

private:
    QVBoxLayout m_layout;
    QTabWidget m_tabs;
    SearchBar* m_searchBar;
};

}

#endif
