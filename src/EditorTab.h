#ifndef EditorTab_h
#define EditorTab_h

#include <QFile>
#include <QGridLayout>
#include <QWidget>
#include <QProcess>
#include <QX11EmbedContainer>

namespace Pecera 
{

class TabbedProcess;
class EditorTabs;

class EditorTab : public QWidget 
{
Q_OBJECT

public:
    EditorTab(QFile*, EditorTabs*);
    ~EditorTab();
    void added();
    QString title();

public slots:
    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess* m_process;
    EditorTabs* m_tabs;
    QFile* m_file;
    QGridLayout* m_layout;
    QX11EmbedContainer* m_x11Embed;
    bool embedded;
};

}

#endif
