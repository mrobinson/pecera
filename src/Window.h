#ifndef Window_h
#define Window_h

#include <QGroupBox>
#include <QVBoxLayout>

#include "EditorTabs.h"

namespace Pecera
{
class SearchBar;

class Window : public QGroupBox
{
    Q_OBJECT

public:
    Window(QWidget* parent=0);
    ~Window();
    EditorTabs* tabs() { return &m_tabs; }

public slots:
    void focusSearchBar();

protected:
    void closeEvent(QCloseEvent*);

private:
    QVBoxLayout m_layout;
    EditorTabs m_tabs;
    SearchBar* m_searchBar;
};

}

#endif
