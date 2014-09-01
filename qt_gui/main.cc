#include "mouse_application.h"

int main(int argc, char *argv[])
{
  QCoreApplication::setApplicationName("func-mouse-config");
  QCoreApplication::setOrganizationName("func-mouse-config");
  MouseApplication app(argc, argv);

  return app.exec();
}

