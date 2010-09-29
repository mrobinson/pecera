#ifndef TabbedProcess_h
#define TabbedProcess_h

#include <iostream>

#include <QProcess>
#include <QX11EmbedContainer>

#include "PeceraApplication.h"
#include "EditorTabs.h"

namespace Pecera
{
  class TabbedProcess : public QX11EmbedContainer
  {
    Q_OBJECT

  public:
    TabbedProcess(EditorTabs* tabWidget, QWidget* parent = 0);
    ~TabbedProcess();

    void setCommand(const QString* command);
    void setArguments(const QStringList* arguments);
    QString getTitle();

  public slots:
    void started();
    void finished(int exitCode, QProcess::ExitStatus exitStatus);

  private:
    int index;
    EditorTabs* m_tabWidget;
    QProcess* process;
    QX11EmbedContainer* container;
    const QString* title;
    const QString* command;
    const QStringList* arguments;

    void launchProcess();
  };
}

#endif
