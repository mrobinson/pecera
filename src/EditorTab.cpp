#include "EditorTabs.h"
#include "Window.h"
#include <QGridLayout>
#include <QFileInfo>

// TODO:
// 1. Figure out how to launch rxvt/vim as the non-top tab and have it still work.
// 2. Push X11 embedding and editor types into subclasses.
// 3. Key listening / focus.


namespace Pecera
{

EditorTab::EditorTab(QFile* file, EditorTabs* tabs) 
    : QWidget()
    , m_process(0)
    , m_tabs(tabs)
    , m_file(file)
    , m_layout(new QGridLayout(this))
    , m_x11Embed(new QX11EmbedContainer())
{
    m_layout->addWidget(m_x11Embed);
    m_x11Embed->show();
}

EditorTab::~EditorTab()
{
    delete m_layout;
}

void EditorTab::added()
{
    if (m_process)
        return;

    m_process = new QProcess(this);

    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(m_process, SIGNAL(started()),
            this, SLOT(processStarted()));

    QStringList arguments;
    arguments << "-into" << QString::number(m_x11Embed->winId());
    arguments << "-e" << "vim" << m_file->fileName();
    m_process->start("/usr/bin/xterm", arguments);
}

void EditorTab::processStarted()
{
}

void EditorTab::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    printf("finished\n");

    int ourIndex = m_tabs->indexOf(this);
    if (ourIndex != -1)
        m_tabs->removeTab(ourIndex);
    m_tabs = 0;
}

QString EditorTab::title()
{
    QFileInfo info(*m_file);
    return info.baseName();
}

}

