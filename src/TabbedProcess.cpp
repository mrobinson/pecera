#include "PeceraApplication.h"
#include "SearchProvider.h"
#include "TabbedProcess.h"
#include <cstdio>
#include <unistd.h>
#include <QFrame>
#include <iostream>

// TODO
// 1. Memory management
// 2. Listen to index changes instead of just trusting that it hasn't changed.
// 3. Figure out how to launch rxvt/vim as the non-top tab and have it still work.
// 4. Implement the provider model for apps.
// 5. Key listening / focus.

namespace Pecera
{
TabbedProcess::TabbedProcess(EditorTabs* tabs, QWidget* parent)
    : QX11EmbedContainer(parent)
    , m_tabWidget(tabs)
    , process(0)
    , container(0)
    , command(0)
    , arguments(0)
    , title(new QString("title"))
{
    show();
}

  TabbedProcess::~TabbedProcess() 
  {
      // TODO:
      // if(process != 0) delete process;
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
    connect(this->process, SIGNAL(started()),
	    this, SLOT(started()));
    m_tabWidget->setCurrentIndex(this->index);
    this->process->start(*this->command, *this->arguments);
    
  }

  void TabbedProcess::started() {
  }

  void TabbedProcess::finished(int exitCode, QProcess::ExitStatus exitStatus) {
    printf("finished\n");
    this->setCommand(0);
    this->setArguments(0);

    int ourIndex = m_tabWidget->indexOf(this->parentWidget());
    if (ourIndex != -1)
        m_tabWidget->removeTab(ourIndex);
    m_tabWidget = 0;
  }

  QString TabbedProcess::getTitle() {
      return *this->title;
  }
}
