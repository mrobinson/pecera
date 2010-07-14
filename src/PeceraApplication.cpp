
#include "PeceraApplication.h"

namespace Pecera {
  
PeceraApplication* PeceraApplication::application = NULL;

PeceraApplication::PeceraApplication(int& argc, char** argv) : 
  QApplication(argc, argv) {
  this->locationBarShortcut = NULL;
}

PeceraApplication& PeceraApplication::getApplication()
{
  return *PeceraApplication::application;
}


PeceraApplication& PeceraApplication::initializeApplication(int& argc, char** argv)
{
  if(PeceraApplication::application == NULL) {
    PeceraApplication::application = new PeceraApplication(argc, argv);
  }

  return *PeceraApplication::application;
}

void PeceraApplication::setLocationBarWindow(QWidget* locationBarWindow) {
  PeceraApplication::application->locationBarWindow = locationBarWindow;
}

void PeceraApplication::reloadLocationShortcut() {
  if(this->locationBarShortcut != NULL) {
    delete this->locationBarShortcut;
    this->locationBarShortcut = NULL;
  }

  this->locationBarShortcut = 
    new QShortcut
    (QKeySequence(QObject::tr("Ctrl+L", "File|Open")), 
     this->locationBarWindow);

  QObject::connect(this->locationBarShortcut, 
		   SIGNAL(activated()), 
		   this, 
		   SLOT(focusSearchBar()));
}

void PeceraApplication::focusSearchBar() {
  static int count = 0;
  std::cout << "focusSearchBar " << count++ << std::endl;
}

}
