#include "EnvironmentEditorProvider.h"

namespace Pecera {

  EnvironmentEditorProvider::EnvironmentEditorProvider(QWidget* parent, QTabWidget* tabs) {
    this->parent = parent;
    this->tabs = tabs;

  }

  EnvironmentEditorProvider::~EnvironmentEditorProvider() {

  }

  void EnvironmentEditorProvider::openFile(QString filename) {
    if(!this->openFiles.contains(filename)) {
      TabbedProcess* tabbedProcess = new TabbedProcess(this->parent);
      tabbedProcess->setTabBar(this->tabs);
    
      QStringList* arguments = new QStringList();
      *arguments << "-embed" << QString::number(tabbedProcess->winId());
      *arguments << "-e" << "vim" << filename;
      tabbedProcess->setCommand(new QString("/usr/bin/urxvt"));
      tabbedProcess->setArguments(arguments);
      QProcess* process = tabbedProcess->launchProcess();

      /*

      connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
	      this, SLOT(finished(int, QProcess::ExitStatus)));
      */

      this->openFiles[filename] = tabbedProcess;
    }
    else {
      TabbedProcess* tabbedProcess = this->openFiles[filename];
      for(int i = 0; i < this->tabs->count(); i++) {
	if(this->tabs->widget(i) == tabbedProcess) {
	  this->tabs->setCurrentIndex(i);
	  break;
	}
      }
    }
  }
}
