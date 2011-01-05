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

void EditorTabs::addEditorTab(EditorTab* tab)
{
    tab->added();
    m_tabs.append(tab);
    this->addTab(tab, tab->title());
}

bool EditorTabs::focusTabIfExists(const QFile& file)
{
    QList<EditorTab*>::iterator i = m_tabs.begin();
    while (i != m_tabs.end()) {
        EditorTab* tab = *i;
        if (tab->file().fileName() != file.fileName())
            continue;
        int index = this->indexOf(tab);
        if (index != -1) {
            this->setCurrentIndex(index);
            return true;
        }
    }
    return false;
}

void EditorTabs::openTabWithFile(const QFile& file)
{
    if (focusTabIfExists(file))
        return;

    this->addEditorTab(new EditorTab(file, this));
}

void EditorTabs::openTabWithFileAndLineNumber(const QFile& file, unsigned long lineNumber)
{
    if (focusTabIfExists(file))
        return;

    EditorTab* tab = new EditorTab(file, this);
    tab->setStartingLineNumber(lineNumber);
    this->addEditorTab(tab);
}

void EditorTabs::openTabWithFileAndRegex(const QFile& file, const QString& regex)
{
    if (focusTabIfExists(file))
        return;

    EditorTab* tab = new EditorTab(file, this);
    tab->setStartingRegex(regex);
    this->addEditorTab(tab);
}

}
