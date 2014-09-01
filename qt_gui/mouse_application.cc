#include "mouse_application.h"

#include "mainwindow.h"

MouseApplication::MouseApplication(int &argc, char ** argv)
  :QApplication(argc, argv),m_mainwindow(new MainWindow)
{
  m_mainwindow->show();
}

MouseApplication::~MouseApplication()
{
  delete m_mainwindow;
}
