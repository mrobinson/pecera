#ifndef EditorTab_h
#define EditorTab_h

#include <QTabWidget>

#include "EditorProvider.h"
#include "TabbedProcess.h"

namespace Pecera 
{

class TabbedProcess;

class EditorTab : public QWidget 
{
Q_OBJECT

public:
    EditorTab(QFile* file);
    ~EditorTab();

    EditorProvider* getProvider();
private:
    TabbedProcess* tabbedProcess;
    EditorProvider* provider;
    QFile* file;
};

}

#endif
