#ifndef EnvironmentEditorProvider_h
#define EnvironmentEditorProvider_h

#include <QHash>
#include <QTabWidget>

#include "EditorProvider.h"
#include "TabbedProcess.h"

namespace Pecera {

  class EnvironmentEditorProvider : public EditorProvider 
  {
  public:
    EnvironmentEditorProvider(QWidget* parent, QTabWidget* tabs);
    ~EnvironmentEditorProvider();

    void openFile(QString filename);
  private:
    QTabWidget* tabs;
    QWidget* parent;
    QHash<QString, TabbedProcess*> openFiles;
  };
}

#endif
