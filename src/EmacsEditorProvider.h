#ifndef EmacsEditorProvider_h
#define EmacsEditorProvider_h

#include <QTabWidget>

namespace Pecera {

  class EmacsEditorProvider 
  {
  public:
    EmacsEditorProvider(QTabWidget* tabs);
    ~EmacsEditorProvider();

    void openFile(const char* filename);
  };
}

#endif
