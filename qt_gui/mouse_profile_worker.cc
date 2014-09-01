#include "mouse_profile_worker.h"

#include "mouse_setup_interface.h"

MouseProfileWorker::MouseProfileWorker(const MouseProfile &profile, int num)
  :m_profile(profile)
   ,m_profile_number(num)
{
}

void MouseProfileWorker::run()
{
  MouseSetupInterface mouse;
  if(!mouse.isOpen())
  {
    emit error(QString(tr("Error accessing mouse : %1"))
        .arg(mouse.getLastErrorMessage().c_str()));
    emit finished();
    return;
  }
  emit message(QString(tr("Sending ...")));
  bool ok = mouse.setProfile(m_profile,m_profile_number);
  if (!ok)
  {
    emit error(QString(tr("Error : %1"))
        .arg(mouse.getLastErrorMessage().c_str()));
    emit finished();
    return;
  }
  emit message(QString(tr("Success")));
  emit finished();
}
