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
    if (this->tabs.contains(file)) {
        // just focus the tab
        EditorTab* tab = this->tabs[file];
        int index = this->indexOf(tab);
        if (index != -1)
            this->setCurrentIndex(index);
    } else {
        EditorTab* tab = new EditorTab(file, this);
        this->addTab(tab, tab->title());
        tab->added();
        this->tabs[file] = tab;
    }
}
}
