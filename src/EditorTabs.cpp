
#include <QGridLayout>

#include "EditorTabs.h"
#include "Window.h"

#include <iostream>

#define DEBUG

#ifndef DEBUG
#define PRINT(label, value)
#else
#define PRINT(label, value) \
std::cout << label << " : line " << __LINE__ << " : " << #value << " : " << value << std::endl;
#endif

namespace Pecera
{

EditorTabs::EditorTabs(QWidget* parent) 
    : QTabWidget(parent) {}

EditorTabs::~EditorTabs() {}

void EditorTabs::removeTab(int index) 
{
    PRINT("EditorTabs::removeTab", index);
    QWidget* page = this->widget(index);
    QTabWidget::removeTab(index);
    delete page;
}

void EditorTabs::setTab(QFile* file)
{
    if(this->tabs.contains(file)) {
	// just focus the tab
	EditorTab* tab = this->tabs[file];
	int index = this->indexOf(tab);
	if(index != -1)
	    this->setCurrentIndex(index);
    }
    else {
	EditorTab* tab = new EditorTab(file);
	this->addTab(tab, "todo"); //tab->getProvider()->getName());
	this->tabs[file] = tab;
    }
}

EditorTab::EditorTab(QFile* file) 
    : QWidget()
{
    std::string filenameString = file->fileName().toStdString();
    PRINT("EditorTab::EditorTab", filenameString);
    this->file = file;

    QGridLayout* layout = new QGridLayout(this);
    PeceraApplication* app = PeceraApplication::instance(); 
    this->tabbedProcess = 
	new TabbedProcess(app->window()->tabs());
    layout->addWidget(this->tabbedProcess);
    QStringList* arguments = new QStringList();
    *arguments << "-into" << QString::number(this->tabbedProcess->winId());
    *arguments << "-e" << "vim" << this->file->fileName();
    this->tabbedProcess->setCommand(new QString("/usr/bin/xterm"));
    this->tabbedProcess->setArguments(arguments);
}

EditorTab::~EditorTab() 
{
    //...
}

EditorProvider* EditorTab::getProvider()
{
    return this->provider;
}

}
