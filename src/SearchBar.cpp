#include <sys/stat.h>

#include <QProcess>
#include <QTabWidget>
#include <QStringList>
#include <QX11EmbedContainer>

#include "SearchBar.h"
#include "SuggestionBox.h"
#include "TabbedProcess.h"

namespace Pecera
{

SearchBar::SearchBar(QWidget* parent)
    : QLineEdit(parent)
    , m_suggestionBox(new SuggestionBox(
        this, Qt::Tool | Qt::Window | Qt::FramelessWindowHint))
{
    connect(this, SIGNAL(textChanged(const QString&)),
        m_suggestionBox, SLOT(lineEditChanged(const QString&)));
    QLineEdit::resize(600, sizeHint().height());
}

SearchBar::~SearchBar()
{
    delete m_suggestionBox;
}

void SearchBar::returned()
{
  struct stat statStruct;

  if(stat(this->text().toStdString().c_str(), &statStruct) == 0) {
    printf("file exists\n");    
    TabbedProcess* tabbedProcess = new TabbedProcess(this->groupBox);
    tabbedProcess->setTabBar(tabs);

    QStringList* arguments = new QStringList();
    *arguments << "-embed" << QString::number(tabbedProcess->winId());
    *arguments << "-e" << "vim" << this->text();
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

}
