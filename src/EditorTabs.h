#ifndef EditorTabs_h
#define EditorTabs_h

#include "EditorTab.h"
#include <QFile>
#include <QHash>
#include <QWidget>
#include <QTabWidget>

namespace Pecera 
{

class EditorTab;

class EditorTabs : public QTabWidget
{
    Q_OBJECT

public:
    EditorTabs(QWidget* parent = 0);
    ~EditorTabs();

    virtual void removeTab(int index);
    void setTab(QFile* file);

private:
    // cases
    //  - a process running in a managed tab
    //  - a process running in its own window... deferred for right now
    QHash<QFile*, EditorTab*> tabs;
};

}

#endif
