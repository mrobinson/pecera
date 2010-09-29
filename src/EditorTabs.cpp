#include "EditorTabs.h"
#include <iostream>

#define DEBUG

#ifndef DEBUG
#define PRINT(label, value)
#else
#define PRINT(label, value)						\
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

}
