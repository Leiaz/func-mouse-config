#include "mainwindow.h"

#include "mouse_config_xml.h"
#include "mouse_profile.h"
#include "mouse_setup_interface.h"

#include <QAction>
#include <QCloseEvent>
#include <QComboBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QSignalMapper>
#include <QStatusBar>
#include <QThread>

#include "mouse_application.h"
#include "mouse_profile_worker.h"
#include "profiles_tab_widget.h"

MainWindow::MainWindow()
  :QMainWindow()
{
  /* QAction */
  QAction *new_action = new QAction(tr("New Profile Tab"), this);
  new_action->setShortcut(QKeySequence(tr("Ctrl+T")));
  QAction *open_tab_action = new QAction(tr("Open in new tab"), this);
  open_tab_action->setShortcut(QKeySequence(tr("Ctrl+O")));
  QAction *save_action = new QAction(tr("Save"), this);
  save_action->setShortcut(QKeySequence(tr("Ctrl+S")));
  QAction *save_as_action = new QAction(tr("Save as"), this);
  save_as_action->setShortcut(QKeySequence(tr("Ctrl+Shift+S")));
  QAction *quit_action = new QAction(tr("Quit"), this);
  quit_action->setShortcut(QKeySequence(tr("Ctrl+Q")));

 QAction *profile_1_action = new QAction(tr("Profile 1"), this);
 QAction *profile_2_action = new QAction(tr("Profile 2"), this);
 QAction *profile_3_action = new QAction(tr("Profile 3"), this);

  /* Menus */
  QMenu *file_menu = menuBar()->addMenu(tr("File"));
  file_menu->addAction(new_action);
  file_menu->addAction(open_tab_action);
  file_menu->addAction(save_as_action);
  file_menu->addAction(save_action);
  file_menu->addAction(quit_action);
  QMenu *switch_profile_menu = menuBar()->addMenu(tr("Switch to profile"));
  switch_profile_menu->addAction(profile_1_action);
  switch_profile_menu->addAction(profile_2_action);
  switch_profile_menu->addAction(profile_3_action);

  /* StatusBar */
  statusBar()->setSizeGripEnabled(false);

  /* Widgets */
  m_profiles_tabs = new ProfilesTabWidget(this);
  // Would be different from settings tabs ... but is ugly
  //m_profiles_tabs->setTabShape(QTabWidget::Triangular);

  m_dpi_selected_profile_1 = new QComboBox(this);
  m_dpi_selected_profile_1->addItem(tr("Slot 1"), 1);
  m_dpi_selected_profile_1->addItem(tr("Slot 2"), 2);
  m_dpi_selected_profile_1->addItem(tr("Slot 3"), 3);

  m_dpi_selected_profile_2 = new QComboBox(this);
  m_dpi_selected_profile_2->addItem(tr("Slot 1"), 1);
  m_dpi_selected_profile_2->addItem(tr("Slot 2"), 2);
  m_dpi_selected_profile_2->addItem(tr("Slot 3"), 3);

  m_dpi_selected_profile_3 = new QComboBox(this);
  m_dpi_selected_profile_3->addItem(tr("Slot 1"), 1);
  m_dpi_selected_profile_3->addItem(tr("Slot 2"), 2);
  m_dpi_selected_profile_3->addItem(tr("Slot 3"), 3);

  QPushButton *apply_dpi_button = new QPushButton(tr("Apply"),
      this);

  m_polling_rate = new QComboBox(this);
  m_polling_rate->addItem(tr("1000 Hz"), 1);
  m_polling_rate->addItem(tr("500 Hz"), 2);
  m_polling_rate->addItem(tr("250 Hz"), 4);
  m_polling_rate->addItem(tr("125 Hz"), 8);
  QPushButton *apply_polling_rate = new QPushButton(tr("Apply"));

  QGridLayout *mouse_setting_layout = new QGridLayout;
  mouse_setting_layout->addWidget(
      new QLabel(tr("Send current profile to mouse slot :")),0,0);
  QPushButton *profile_1_button = new QPushButton(tr("Profile 1"));
  QPushButton *profile_2_button = new QPushButton(tr("Profile 2"));
  QPushButton *profile_3_button = new QPushButton(tr("Profile 3"));
  mouse_setting_layout->addWidget(profile_1_button, 0,1);
  mouse_setting_layout->addWidget(profile_2_button, 0,2);
  mouse_setting_layout->addWidget(profile_3_button, 0,3);

  mouse_setting_layout->addWidget(new QLabel(tr("Selected DPI : ")), 1,0);
  mouse_setting_layout->addWidget(m_dpi_selected_profile_1, 1,1);
  mouse_setting_layout->addWidget(m_dpi_selected_profile_2, 1,2);
  mouse_setting_layout->addWidget(m_dpi_selected_profile_3, 1,3);
  mouse_setting_layout->addWidget(apply_dpi_button, 1,4);

  mouse_setting_layout->addWidget(new QLabel(tr("Polling rate : ")),2,0);
  QHBoxLayout *polling_rate_layout = new QHBoxLayout;
  polling_rate_layout->addWidget(m_polling_rate);
  polling_rate_layout->addWidget(apply_polling_rate);
  polling_rate_layout->addStretch();

  mouse_setting_layout->addLayout(polling_rate_layout, 2,1, 1,4);

  /* Central widget */
  QWidget *central_widget = new QWidget;
  QVBoxLayout *central_layout = new QVBoxLayout;
  central_layout->addWidget(m_profiles_tabs);
  central_layout->addLayout(mouse_setting_layout);
  central_widget->setLayout(central_layout);

  /* Mainwindow Widget */
  setCentralWidget(central_widget);

  /* Init */
  QSettings settings;
  m_profiles_tabs->tryOpenFiles(settings.value("profiles")
      .toStringList());
  if(m_profiles_tabs->count() == 0)
  {
    newProfile();
  }

  /* connect */
  connect(new_action, &QAction::triggered, this, &MainWindow::newProfile);
  connect(open_tab_action, &QAction::triggered,
      this, &MainWindow::openInNewTab);
  connect(save_action, &QAction::triggered,
      m_profiles_tabs, &ProfilesTabWidget::saveCurrentProfile);
  connect(save_as_action, &QAction::triggered,
      m_profiles_tabs, &ProfilesTabWidget::saveCurrentProfileAs);
  connect(quit_action, &QAction::triggered, this, &MainWindow::close);

  QSignalMapper *profile_button_mapper = new QSignalMapper(this);
  profile_button_mapper->setMapping(profile_1_button, 1);
  profile_button_mapper->setMapping(profile_2_button, 2);
  profile_button_mapper->setMapping(profile_3_button, 3);

  QSignalMapper *profile_action_mapper = new QSignalMapper(this);
  profile_action_mapper->setMapping(profile_1_action, 1);
  profile_action_mapper->setMapping(profile_2_action, 2);
  profile_action_mapper->setMapping(profile_3_action, 3);

  void (QSignalMapper:: *mapVoidSlot) (void) = &QSignalMapper::map;
  connect(profile_1_button, &QPushButton::clicked,
      profile_button_mapper, mapVoidSlot);
  connect(profile_2_button, &QPushButton::clicked,
      profile_button_mapper, mapVoidSlot);
  connect(profile_3_button, &QPushButton::clicked,
      profile_button_mapper, mapVoidSlot);

  connect(profile_1_action, &QAction::triggered,
      profile_action_mapper, mapVoidSlot);
  connect(profile_2_action, &QAction::triggered,
      profile_action_mapper, mapVoidSlot);
  connect(profile_3_action, &QAction::triggered,
      profile_action_mapper, mapVoidSlot);

  connect(profile_button_mapper,
      (void (QSignalMapper:: *) (int)) &QSignalMapper::mapped,
      this, &MainWindow::sendProfileToMouse);

  connect(profile_action_mapper,
      (void (QSignalMapper:: *) (int)) &QSignalMapper::mapped,
      this, &MainWindow::switchToProfile);

  connect(apply_dpi_button, &QPushButton::clicked,
      this, &MainWindow::changeSelectedDPI);
  connect(apply_polling_rate, &QPushButton::clicked,
      this, &MainWindow::changePollingRate);
}

/**
 * Remember opened files on close.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
  QSettings settings;
  settings.setValue("profiles", m_profiles_tabs->getOpenedFiles());
  event->accept();
}

void MainWindow::sendProfileToMouse(int profile_num)
{
  QThread* thread = new QThread;
  MouseProfileWorker *profile_worker =
    new MouseProfileWorker(*(m_profiles_tabs->getCurrentProfile()),
        profile_num);

  QProgressBar *busy_bar = new QProgressBar(statusBar());
  statusBar()->addPermanentWidget(busy_bar);
  busy_bar->setMinimum(0);
  busy_bar->setMaximum(0);
  busy_bar->setFormat(tr("In Progress"));


  profile_worker->moveToThread(thread);

  connect(profile_worker, &MouseProfileWorker::error,
      this, &MainWindow::displayMouseError);

  connect(thread, &QThread::started,
      profile_worker, &MouseProfileWorker::run);
  connect(profile_worker, &MouseProfileWorker::finished,
      thread, &QThread::quit);
  connect(profile_worker, &MouseProfileWorker::finished,
      profile_worker, &MouseProfileWorker::deleteLater);
  connect(thread, &QThread::finished,
      thread, &QThread::deleteLater);
  connect(profile_worker, &MouseProfileWorker::finished,
      busy_bar, &QProgressBar::deleteLater);
  connect(profile_worker, &MouseProfileWorker::message,
      statusBar(),
      (void (QStatusBar::*) (const QString&))&QStatusBar::showMessage);

  thread->start();
}

void MainWindow::changeSelectedDPI()
{
  MouseSetupInterface mouse;
  if(!mouse.isOpen())
  {
    displayMouseError(QString(tr("Error accessing mouse : %1"))
        .arg(mouse.getLastErrorMessage().c_str()));
    return;
  }
  statusBar()->showMessage(tr("Sending ..."));
  bool ok = mouse.setSelectedDPI(m_dpi_selected_profile_1->currentData().toInt(),
      m_dpi_selected_profile_2->currentData().toInt(),
      m_dpi_selected_profile_3->currentData().toInt());
  if (!ok)
  {
    displayMouseError(QString(tr("Error : %1"))
        .arg(mouse.getLastErrorMessage().c_str()));
    return;
  }
  statusBar()->showMessage(tr("OK"));
}

void MainWindow::changePollingRate()
{
   MouseSetupInterface mouse;
  if(!mouse.isOpen())
  {
    displayMouseError(QString(tr("Error accessing mouse : %1"))
        .arg(mouse.getLastErrorMessage().c_str()));
    return;
  }
  statusBar()->showMessage(tr("Sending ..."));
  bool ok = mouse.setPollingInterval(
      m_polling_rate->currentData().toInt());
  if (!ok)
  {
    displayMouseError(QString(tr("Error : %1"))
        .arg(mouse.getLastErrorMessage().c_str()));
    return;
  }
  statusBar()->showMessage(tr("OK"));
}

void MainWindow::switchToProfile(int profile_num)
{
  if ((profile_num < 1)||(profile_num > 3))
    return;
  MouseSetupInterface mouse;
  if(!mouse.isOpen())
  {
    displayMouseError(QString(tr("Error accessing mouse : %1"))
        .arg(mouse.getLastErrorMessage().c_str()));
    return;
  }
  statusBar()->showMessage(tr("Sending ..."));
  bool ok = mouse.switchToProfile(profile_num);
  if (!ok)
  {
    displayMouseError(QString(tr("Error : %1"))
        .arg(mouse.getLastErrorMessage().c_str()));
    return;
  }
  statusBar()->showMessage(tr("OK"));
}

void MainWindow::displayMouseError(const QString &error)
{
  QMessageBox::critical(this, tr("Error"), error);
  statusBar()->showMessage(tr("Failed"));
}

void MainWindow::newProfile()
{
  int i;
  i = m_profiles_tabs->addProfileTab();
  m_profiles_tabs->setCurrentIndex(i);
}

void MainWindow::openInNewTab()
{
  QString filename = QFileDialog::getOpenFileName(this,
      tr("Open profile"), "", tr("XML Files (*.xml)"));
  if(!filename.isEmpty())
  {
    int i = m_profiles_tabs->addProfileTab(filename);
    m_profiles_tabs->setCurrentIndex(i);
  }
}
