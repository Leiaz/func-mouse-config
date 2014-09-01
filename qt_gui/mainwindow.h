#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ProfilesTabWidget;
class QComboBox;

class MainWindow :public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow();

  protected:
    void closeEvent(QCloseEvent *event);

  private slots:
    void sendProfileToMouse(int profile_num);
    void changeSelectedDPI();
    void changePollingRate();
    void switchToProfile(int profile_num);
    void displayMouseError(const QString &error);
    void newProfile();
    void openInNewTab();

  private:
    ProfilesTabWidget *m_profiles_tabs;
    QComboBox *m_dpi_selected_profile_1;
    QComboBox *m_dpi_selected_profile_2;
    QComboBox *m_dpi_selected_profile_3;
    QComboBox *m_polling_rate;
};
#endif
