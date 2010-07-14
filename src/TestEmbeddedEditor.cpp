#include "Project.h"
#include "SearchBar.h"
#include "SuggestionBox.h"
#include <cstdio>
#include <QApplication>
#include <QGroupBox>
#include <QLineEdit>
#include <QProcess>
#include <QObject>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QX11EmbedContainer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Pecera");

    QString projectName("myProject");
    QDir projectDir("/home/martin/app/titanium_desktop");
    Pecera::Project p(projectName, projectDir);
    p.scanRoot(); 
    p.save(); 

  QGroupBox groupBox;
  QVBoxLayout layout;

  QTabWidget tabs;
  Pecera::SearchBar searchBar;
  searchBar.tabs = &tabs;
  searchBar.groupBox = &groupBox;
  searchBar.connect(&searchBar, SIGNAL(returnPressed()),
		    &searchBar, SLOT(returned()));
    searchBar.suggestionBox()->setProject(&p);

  layout.addWidget(&searchBar);
  layout.addWidget(&tabs);

  /*
  QApplication app(argc, argv);
  QX11EmbedContainer container(0);
  container.show();


  */

  groupBox.setLayout(&layout);
  groupBox.show();
  groupBox.setGeometry(5, 5, 800, 600);
  return app.exec();
}
