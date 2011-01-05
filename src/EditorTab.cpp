#include "EditorTabs.h"
#include "Window.h"
#include <QFile>
#include <QFileInfo>
#include <QGridLayout>
#include <QTextStream>
#include <QTimer>

// TODO:
// 1. Figure out how to launch rxvt/vim as the non-top tab and have it still work.
// 2. Push X11 embedding and editor types into subclasses.
// 3. Key listening / focus.


namespace Pecera
{

EditorTab::EditorTab(const QFile& file, EditorTabs* tabs) 
    : QWidget()
    , m_process(0)
    , m_tabs(tabs)
    , m_file(file.fileName())
    , m_startingLineNumber(0)
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

    QTimer* processStartTimer = new QTimer(this);
    connect(processStartTimer, SIGNAL(timeout()), this, SLOT(startProcess()));
    processStartTimer->setSingleShot(true);
    processStartTimer->start(0);
}

void EditorTab::startProcess()
{
    m_process = new QProcess(this);
    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(m_process, SIGNAL(started()),
            this, SLOT(processStarted()));

    QStringList arguments;
    arguments << "-into" << QString::number(m_x11Embed->winId());
    arguments << "-e" << "vim" << m_file.fileName();

    if (m_startingLineNumber > 0) {
        QString lineNumberArgument;
        QTextStream(&lineNumberArgument) << "+" << m_startingLineNumber;
        arguments << lineNumberArgument;
    } else if (m_startingRegex.length()) {
        QString regexArgument;
        QTextStream(&regexArgument) << "+" << m_startingRegex;
        arguments << regexArgument;
    }

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
    QFileInfo info(m_file);
    return info.baseName();
}

}

