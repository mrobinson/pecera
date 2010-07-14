
#include "PeceraApplication.h"
#include <X11/Xlib.h>
#include <QX11Info>
#include <iostream>

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

bool PeceraApplication::x11EventFilter(XEvent* event) {
  Display *display = QX11Info::display();

  if(event->type == KeyPress || event->type == KeyRelease) {
    //std::cout << event->xkey.keycode << std::endl;
  }
  if (event->type == ClientMessage) {
    std::cout << "ClientMessage" << std::endl;
    Atom atom = XInternAtom(display, "_XEMBED", FALSE);
    if(event->xclient.message_type == atom) {
      std::cout << "XEMBED" << std::endl;
    }
  }

  QApplication::x11EventFilter(event);
}

}
