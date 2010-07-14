#ifndef TabbedProcess_h
#define TabbedProcess_h

#include <iostream>

#include <QProcess>
#include <QTabWidget>
#include <QX11EmbedContainer>

#include "PeceraApplication.h"

namespace Pecera
{
  class TabbedProcess : public QX11EmbedContainer
  {
    Q_OBJECT

  public:
    TabbedProcess(QWidget* parent = 0);
    ~TabbedProcess();

    void setTabBar(QTabWidget* tabBar);
    void setCommand(const QString* command);
    void setArguments(const QStringList* arguments);

  public slots:
    void started();
    void finished(int exitCode, QProcess::ExitStatus exitStatus);

  private:
    int index;
    QTabWidget* tabBar;
    QProcess* process;
    QX11EmbedContainer* container;
    const QString* title;
    const QString* command;
    const QStringList* arguments;

    void launchProcess();
  };
}

#endif
