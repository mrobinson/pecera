#ifndef PeceraApplication_h
#define PeceraApplication_h

#include <iostream>

#include <QApplication>
#include <QObject>
#include <QShortcut>

namespace Pecera {

class PeceraApplication : public QApplication {
  Q_OBJECT

public:
  static PeceraApplication& getApplication();
  static PeceraApplication& initializeApplication(int& argc, char** argv);
  void setLocationBarWindow(QWidget* locationBarWindow);
  void reloadLocationShortcut();

public slots:
  void focusSearchBar();

private: 
  static PeceraApplication* application;
  QShortcut* locationBarShortcut;
  QWidget* locationBarWindow;
 
  PeceraApplication(int& argc, char** argv);
  ~PeceraApplication() {} 
  PeceraApplication(const PeceraApplication &);
  PeceraApplication& operator=(const PeceraApplication &);
};

}

#endif
