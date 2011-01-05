#ifndef EditorTabs_h
#define EditorTabs_h

#include "EditorTab.h"
#include <QFile>
#include <QHash>
#include <QWidget>
#include <QTabWidget>

namespace Pecera 
{

class EditorTab;

class EditorTabs : public QTabWidget
{
    Q_OBJECT

public:
    EditorTabs(QWidget* parent = 0);
    ~EditorTabs();

    virtual void removeTab(int index);
    void openTabWithFile(const QFile&);
    void openTabWithFileAndLineNumber(const QFile& file, unsigned long lineNumber);
    void openTabWithFileAndRegex(const QFile& file, const QString& regex);

private:
    void addEditorTab(EditorTab*);
    bool focusTabIfExists(const QFile&);
    QList<EditorTab*> m_tabs;
};

}

#endif
