#ifndef EditorTabs_h
#define EditorTabs_h

#include <QTabWidget>

namespace Pecera 
{

class EditorTabs : public QTabWidget
{
    Q_OBJECT

public:
    EditorTabs(QWidget* parent = 0);
    ~EditorTabs();

    virtual void removeTab(int index);
};

}

#endif
