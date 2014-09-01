#include "profiles_tab_widget.h"

#include "mouse_config_xml.h"

#include <QInputDialog>
#include <QTabBar>
#include <QMessageBox>
#include <QFileDialog>

#include "profile_widget.h"

ProfilesTabWidget::ProfilesTabWidget(QWidget *parent):
  QTabWidget(parent)
{
  setTabsClosable(true);

  connect(this, &ProfilesTabWidget::tabCloseRequested,
      this, &ProfilesTabWidget::closeProfileTab);
  connect(this, &ProfilesTabWidget::tabBarDoubleClicked,
      this, &ProfilesTabWidget::changeProfileName);
}

std::shared_ptr<MouseProfile> ProfilesTabWidget::getCurrentProfile() const
{
  ProfileWidget *widget = getCurrentProfileWidget();
  if (widget)
  {
    return widget->getProfile();
  }
  return nullptr;
}

std::shared_ptr<MouseProfile> ProfilesTabWidget::getProfile(int index) const
{
  ProfileWidget *widget = getProfileWidget(index);
  if (widget)
  {
    return widget->getProfile();
  }
  return nullptr;
}

int ProfilesTabWidget::addProfileTab(std::shared_ptr<MouseProfile> profile)
{
  ProfileWidget * profile_widget = new ProfileWidget(profile, this);
  int index = addTab(profile_widget,
      profile_widget->getIcon(),
      QString::fromStdString(profile_widget->getProfile()->getName()));
  connect(profile_widget, &ProfileWidget::iconChanged,
      this, &ProfilesTabWidget::updateTabIcon);
  connect(profile_widget, &ProfileWidget::profileModified,
      this, &ProfilesTabWidget::updateTabModified);
  return index;
}

int ProfilesTabWidget::addProfileTab(const QString &filepath)
{
  MouseConfigXml xml_doc;
  if(xml_doc.loadFile(filepath.toStdString()))
  {
    int index = addProfileTab(xml_doc.extractProfile());
    getProfileWidget(index)->setFilename(filepath);
    return index;
  }
  return -1;
}

bool ProfilesTabWidget::saveProfile(int index, const QString &filename)
{
  MouseConfigXml xml_doc;
  bool success;
  std::shared_ptr<MouseProfile> profile = getProfile(index);
  if(!profile)
  {
    return false;
  }
  xml_doc.writeProfile(*profile);
  success = xml_doc.saveToFile(filename.toStdString());
  if (success)
  {
    getProfileWidget(index)->setFilename(filename);
    setTabModified(index,false);
  }
  return success;
}

QStringList ProfilesTabWidget::getOpenedFiles() const
{
  QStringList open_files;
  for (int i=0;i<count();++i)
  {
    ProfileWidget *w = getProfileWidget(i);
    if (!w->getFilename().isEmpty())
    {
      open_files << w->getFilename();
    }
  }
  return open_files;
}

void ProfilesTabWidget::tryOpenFiles(QStringList filenames)
{
  for(auto file: filenames)
  {
    addProfileTab(file);
  }
}

void ProfilesTabWidget::saveCurrentProfile()
{
  ProfileWidget *widget = getCurrentProfileWidget();
  if (!widget)
    return;

  if (widget->getFilename().isEmpty())
  {
    saveCurrentProfileAs();
  } else {
    if (!saveProfile(currentIndex(), widget->getFilename()))
    {
    QMessageBox::critical(this, tr("Error"),
        QString(tr("Error writing file %1")).arg(widget->getFilename()));
    }
  }
}

void ProfilesTabWidget::saveCurrentProfileAs()
{
  QStringList filenames;
  QFileDialog filedialog(this);
  filedialog.setFileMode(QFileDialog::AnyFile);
  filedialog.setWindowTitle(tr("Save profile"));
  filedialog.setAcceptMode(QFileDialog::AcceptSave);
  filedialog.setDefaultSuffix("xml");
  filedialog.setNameFilter(tr("XML Files (*.xml)"));
  if (filedialog.exec())
  {
    filenames = filedialog.selectedFiles();
  } else {
    return;
  }

  if(!saveProfile(currentIndex(),filenames.first()))
  {
    QMessageBox::critical(this, tr("Error"),
        QString(tr("Error writing file %1")).arg(filenames.first()));
  }
}

void ProfilesTabWidget::closeProfileTab(int index)
{
  QWidget *w = widget(index);
  removeTab(index);
  delete w;
}

void ProfilesTabWidget::changeProfileName(int index)
{
  bool ok;
  QString new_name = QInputDialog::getText(this,
      tr("Profile name"), tr("Change profile name :"),
      QLineEdit::Normal,
      QString::fromStdString(getProfile(index)->getName()),
      &ok);
  if (ok && !new_name.isEmpty())
  {
    getProfile(index)->setName(new_name.toStdString());
    setTabText(index, new_name);
  }
}

void ProfilesTabWidget::updateTabIcon()
{
  int index = indexOf(qobject_cast<QWidget*>(sender()));
  setTabIcon(index, getProfileWidget(index)->getIcon());
}

void ProfilesTabWidget::updateTabModified()
{
  int index = indexOf(qobject_cast<QWidget*>(sender()));
  setTabModified(index,true);
}

void ProfilesTabWidget::setTabModified(int index, bool modified)
{
  if (modified)
  {
    tabBar()->setTabTextColor(index, QColor(Qt::blue));
  } else {
    tabBar()->setTabTextColor(index, QColor()); // Invalid QColor = default style
  }
}

ProfileWidget *ProfilesTabWidget::getCurrentProfileWidget()
{
  return static_cast<ProfileWidget*>(currentWidget());
}

ProfileWidget *ProfilesTabWidget::getCurrentProfileWidget() const
{
  return static_cast<ProfileWidget*>(currentWidget());
}

ProfileWidget *ProfilesTabWidget::getProfileWidget(int index)
{
  return static_cast<ProfileWidget*>(widget(index));
}

ProfileWidget *ProfilesTabWidget::getProfileWidget(int index) const
{
  return static_cast<ProfileWidget*>(widget(index));
}
