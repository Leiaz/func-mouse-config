#ifndef MOUSE_PROFILE_WORKER_H
#define MOUSE_PROFILE_WORKER_H

#include "mouse_profile.h"

#include <QObject>

class MouseProfileWorker : public QObject
{
  Q_OBJECT

  public:
    MouseProfileWorker(const MouseProfile &profile, int num);

  public slots:
    void run();

  signals:
    void finished();
    void error(QString error);
    void message(QString msg);

  private:
    MouseProfile m_profile;
    int m_profile_number;
};
#endif
