#include "Project.h"
#include "SearchBar.h"
#include "SuggestionBox.h"
#include "PeceraApplication.h"

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
  QString projectName("slideshow");
  QDir projectDir("/home/martin/app/titanium_desktop");
  Pecera::Project p(projectName, projectDir);
  p.scanRoot(); 
  p.save(); 

  Pecera::PeceraApplication& app = Pecera::PeceraApplication::initializeApplication(argc, argv);
  
  QGroupBox groupBox;
  QVBoxLayout layout;

  app.setLocationBarWindow(&groupBox);
  app.reloadLocationShortcut();

  QTabWidget tabs;
  Pecera::SearchBar searchBar;
  searchBar.tabs = &tabs;
  searchBar.groupBox = &groupBox;
  searchBar.connect(&searchBar, SIGNAL(returnPressed()),
		    &searchBar, SLOT(returned()));
  searchBar.suggestionBox()->setProject(&p);

  layout.addWidget(&searchBar);
  layout.addWidget(&tabs);

  groupBox.setLayout(&layout);
  groupBox.show();
  groupBox.setGeometry(5, 5, 800, 600);

  return app.exec();
}

