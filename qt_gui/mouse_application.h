#ifndef MOUSE_APPLICATION_H
#define MOUSE_APPLICATION_H

#include <QApplication>

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<MouseApplication*>(QCoreApplication::instance()))

class MainWindow;

class MouseApplication : public QApplication
{
  public:
    MouseApplication(int &argc, char **argv);
    ~MouseApplication();

  private:
    MainWindow *m_mainwindow;
};
#endif
