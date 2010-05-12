#include <QApplication>

#include <cstdio>
#include <QLineEdit>

#include <QX11EmbedContainer>
#include <QProcess>
#include <QGroupBox>
#include <QObject>
#include <QTabWidget>
#include <QAction>
#include <QVBoxLayout>

#include "SearchBar.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QGroupBox groupBox;
  QVBoxLayout layout;

  QTabWidget tabs;
  Pecera::SearchBar searchBar;
  searchBar.tabs = &tabs;
  searchBar.eep = new Pecera::EnvironmentEditorProvider(&groupBox, &tabs);
  searchBar.groupBox = &groupBox;
  searchBar.connect(&searchBar, SIGNAL(returnPressed()),
		    &searchBar, SLOT(returned()));

  layout.addWidget(&searchBar);
  layout.addWidget(&tabs);

  groupBox.setLayout(&layout);
  groupBox.show();
  groupBox.setGeometry(5, 5, 800, 600);

  return app.exec();
}
