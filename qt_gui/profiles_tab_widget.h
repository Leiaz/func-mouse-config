#ifndef PROFILES_TAB_WIDGET_H
#define PROFILES_TAB_WIDGET_H

#include "mouse_profile.h"

#include <QTabWidget>

class ProfileWidget;

class ProfilesTabWidget : public QTabWidget
{
  Q_OBJECT

  public:
    ProfilesTabWidget(QWidget *parent = 0);
    std::shared_ptr<MouseProfile> getCurrentProfile() const;
    std::shared_ptr<MouseProfile> getProfile(int index) const;
    /**
     * Returns the new tab's index (like QTabWidget::addTab)
     * With \param profile = nullptr : a new default profile
     * is created.
     */
    int addProfileTab(std::shared_ptr<MouseProfile> profile = nullptr);
    /**
     * \return -1 if the file couldn't be read.
     */
    int addProfileTab(const QString &filepath);
    bool saveProfile(int index, const QString &filename);

    QStringList getOpenedFiles() const;
    void tryOpenFiles(QStringList filenames);

  public slots:
    void saveCurrentProfile();
    void saveCurrentProfileAs();

  private slots:
    void closeProfileTab(int index);
    void changeProfileName(int index);
    void updateTabIcon();
    void updateTabModified();

  private:
    /**
     * Change the appearance of the tab to show unsaved changes
     */
    void setTabModified(int index, bool modified);

    /**
     * Return nullptr if there are no tabs.
     */
    ProfileWidget *getCurrentProfileWidget();
    ProfileWidget *getCurrentProfileWidget() const;
    /**
     * Returns nullptr if index out of range.
     */
    ProfileWidget *getProfileWidget(int index);
    ProfileWidget *getProfileWidget(int index) const;
};
#endif
