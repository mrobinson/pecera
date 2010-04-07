#include <KCmdLineArgs>
#include <KApplication>
#include <KAboutData>
#include <QApplication>
#include <QDirModel>
#include <QTreeView>
#include <QSplitter>
#include <kde_terminal_interface.h>
#include <klibloader.h>
#include <kshell.h>
#include <kparts/part.h>
#include <KUrl>

#include "SearchBar.h"

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

    KCmdLineArgs::init( argc, argv, &aboutData );
    KApplication app;
    //QSplitter *splitter = new QSplitter();

    //QDirModel *model = new QDirModel;
    //QTreeView *tree = new QTreeView(splitter);
    //tree->setModel(model);
    //tree->resize(300, 500);

    //KPluginFactory *factory = KPluginLoader("libkonsolepart").factory();
    //KParts::ReadOnlyPart *part = factory ? (factory->create<KParts::ReadOnlyPart>(splitter)) : 0;

    //if (!part) {
    //    printf("Failed to initialize part\n");
    //    return 0;
    //}

    //TerminalInterface* m_terminal = qobject_cast<TerminalInterface*>(part);
    //if (!m_terminal) {
    //    printf("Failed to initialize terminal\n");
    //    return 0;
    //}

    //m_terminal->showShellInDir(KUrl().path());
    //m_terminal->sendInput("cd " + KShell::quoteArg(KUrl().path()) + '\n');
    //m_terminal->sendInput("clear\n");
    //QWidget *m_widget = part->widget();
    //m_widget->resize(300, 500);
    //m_widget->show();

    //splitter->show();
    Pecera::SearchBar searchBar;
    searchBar.show();
    return app.exec();
}
