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
    EditorTab(const QFile&, EditorTabs*);
    ~EditorTab();
    void added();
    QString title();

    void setStartingLineNumber(unsigned long lineNumber) { m_startingLineNumber = lineNumber; }
    void setStartingRegex(const QString& regex) { m_startingRegex = regex; }

    const QFile& file() { return m_file; }

public slots:
    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void startProcess();

private:
    QProcess* m_process;
    EditorTabs* m_tabs;
    QFile m_file;
    unsigned long m_startingLineNumber;
    QString m_startingRegex;
    QGridLayout* m_layout;
    QX11EmbedContainer* m_x11Embed;
    bool embedded;
};

}

#endif
