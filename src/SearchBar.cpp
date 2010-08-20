#include "Project.h"
#include "SearchBar.h"
#include "SuggestionBox.h"
#include "TabbedProcess.h"
#include <QFile>
#include <QProcess>
#include <QTabWidget>
#include <QStringList>
#include <QX11EmbedContainer>
#include <sys/stat.h>

namespace Pecera
{

SearchBar::SearchBar(Project* project, QWidget* parent)
    : QLineEdit(parent)
    , m_project(project)
    , m_suggestionBox(new SuggestionBox(this, project))
{
    connect(this, SIGNAL(textChanged(const QString&)),
        m_suggestionBox, SLOT(searchBarChanged(const QString&)));
    connect(this, SIGNAL(returnPressed()),
        this, SLOT(returned()));
    QLineEdit::resize(600, sizeHint().height());
}

SearchBar::~SearchBar()
{
    delete m_suggestionBox;
}

void SearchBar::returned()
{
    QFile targetFile(m_project->getAbsolutePath(this->text()));
    if (!targetFile.exists())
        return;

    TabbedProcess* tabbedProcess = new TabbedProcess(this->groupBox);
    tabbedProcess->setTabBar(tabs);

    QStringList* arguments = new QStringList();
    *arguments << "-embed" << QString::number(tabbedProcess->winId());
    *arguments << "-e" << "vim" << targetFile.fileName();
    tabbedProcess->setCommand(new QString("/usr/bin/urxvt"));
    tabbedProcess->setArguments(arguments);

    /*
    QStringList* arguments = new QStringList();
    *arguments << "--parent-id" << QString::number(tabbedProcess->winId());
    *arguments << "-Q";
    *arguments << "--file" << this->text();
    tabbedProcess->setCommand(new QString("/usr/bin/emacs-snapshot"));
    tabbedProcess->setArguments(arguments);
    */
}

}
