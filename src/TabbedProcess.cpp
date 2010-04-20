#include <cstdio>

#include "TabbedProcess.h"

// TODO
// 1. Memory management
// 2. Listen to index changes instead of just trusting that it hasn't changed.
// 3. Figure out how to launch rxvt/vim as the non-top tab and have it still work.
// 4. Implement the provider model for apps.
// 5. Key listening / focus.

namespace Pecera
{
  TabbedProcess::TabbedProcess(QWidget* parent) : QX11EmbedContainer(parent) {
    this->tabBar = 0;
    this->process = 0;
    this->container = 0;
    this->command = 0;
    this->arguments = 0;
    this->title = new QString("title");
    this->show();
  }

  TabbedProcess::~TabbedProcess() {
    // TODO:
    // if(process != 0) delete process;
  }

  void TabbedProcess::setTabBar(QTabWidget* tabBar) {
    if(this->tabBar == tabBar) return;
    if(this->tabBar != 0) {
      for(int i = 0; i < this->tabBar->count(); i++) {
	if(this->tabBar->widget(i) == this) {
	  this->tabBar->removeTab(i);
	  break;
	}
      }
    }
    this->tabBar = tabBar;
    if(this->tabBar != 0) {
      this->index = this->tabBar->addTab(this, *this->title);
    }
  }

  void TabbedProcess::setCommand(const QString* command) {
    this->command = command;
    if((this->command != 0) && (this->arguments != 0))
      this->launchProcess();
  }

  void TabbedProcess::setArguments(const QStringList* arguments) {
    this->arguments = arguments;
    if((this->command != 0) && (this->arguments != 0))
      this->launchProcess();
  }

  void TabbedProcess::launchProcess() {
    this->process = new QProcess(this);
    connect(this->process, SIGNAL(finished(int, QProcess::ExitStatus)),
	    this, SLOT(finished(int, QProcess::ExitStatus)));
    this->tabBar->setCurrentIndex(this->index);
    this->process->start(*this->command, *this->arguments);
  }

  void TabbedProcess::finished(int exitCode, QProcess::ExitStatus exitStatus) {
    printf("finished\n");
    this->setCommand(0);
    this->setArguments(0);
    this->setTabBar(0);
  }
}
