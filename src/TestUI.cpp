#include <KCmdLineArgs>
#include <KApplication>
#include <KAboutData>
#include <QApplication>
#include <QDirModel>
#include <QTreeView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QPushButton>
#include <kde_terminal_interface.h>
#include <klibloader.h>
#include <kshell.h>
#include <kparts/part.h>
#include <KUrl> 
#include "SearchBar.h"

QWidget* createTerminalWidget(QWidget* parent = 0)
{
    KPluginFactory* factory = KPluginLoader("libkonsolepart").factory();
    KParts::ReadOnlyPart* part = factory ? (factory->create<KParts::ReadOnlyPart>(parent)) : 0;

    if (!part) {
        printf("Failed to initialize part\n");
        return 0;
    }

    TerminalInterface* terminal = qobject_cast<TerminalInterface*>(part);
    if (!terminal) {
        printf("Failed to initialize terminal\n");
        return 0;
    }

    terminal->showShellInDir(KUrl().path());
    terminal->sendInput("cd " + KShell::quoteArg(KUrl().path()) + '\n');
    terminal->sendInput("clear\n");
    return part->widget();
}

int main(int argc, char *argv[])
{
    KAboutData aboutData(
        "tutorial1",
         0,
         ki18n("Tutorial 1"),
         "1.0",
         ki18n("Displays a KMessageBox popup"),
         KAboutData::License_GPL,
         ki18n("(c) 2007"),
         ki18n("Some text..."),
         "http://tutorial.com/",
         "submit@bugs.kde.org");
    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication app;

    QSplitter* splitter = new QSplitter();
    QDirModel* model = new QDirModel;
    QTreeView* tree = new QTreeView(splitter);
    tree->setModel(model);
    tree->resize(100, 800);

    QFrame* frame = new QFrame(splitter);
    QBoxLayout* boxLayout = new QVBoxLayout(frame);
    QWidget* searchBar = new Pecera::SearchBar();
    QWidget* terminal = createTerminalWidget(frame);

    boxLayout->addWidget(searchBar);
    boxLayout->addWidget(terminal);
    frame->resize(600, 500);
    //terminal->show();

    splitter->resize(700, 500);
    splitter->show();
    return app.exec();
}
