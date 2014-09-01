#include "profile_widget.h"

#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QSignalMapper>
#include <QSlider>
#include <QTabWidget>
#include <QVBoxLayout>

#include "button_setting_widget.h"
#include "dpi_widget.h"
#include "led_widget.h"

ProfileWidget::ProfileWidget(std::shared_ptr<MouseProfile> p,
    QWidget *parent):
  QWidget(parent),m_profile(p)
{

  if (!m_profile)
  {
    m_profile = std::shared_ptr<MouseProfile>(new MouseProfile);
  }

  m_wheel_led_widget = new LEDWidget(this);
  m_side_led_widget = new LEDWidget(this);

  m_dpi1_widget = new DPIWidget(MIN_DPI, MAX_DPI, DPI_STEP, this);
  m_dpi2_widget = new DPIWidget(MIN_DPI, MAX_DPI, DPI_STEP,this);
  m_dpi3_widget = new DPIWidget(MIN_DPI, MAX_DPI, DPI_STEP,this);
  m_dpiia_widget = new DPIWidget(MIN_DPI, MAX_DPI, DPI_STEP,this);

  // Angle snapping
  m_angle_snapping_check = new QCheckBox(tr("Angle snapping"), this);
  // Lift off distance
  m_lift_off_distance_slider = new QSlider(Qt::Horizontal, this);
  m_lift_off_distance_slider->setMinimum(0);
  m_lift_off_distance_slider->setMaximum(2);

  // Macros/ buttons
  for(int i = 0;i<BUTTON_COUNT;i++)
  {
    m_button_widget[i]= new ButtonSettingWidget(this);
  }
  // Other init
  setWidgets();
  drawIcon();

  // Layout
  QGroupBox *wheel_led_box = new QGroupBox(this);
  wheel_led_box->setTitle(tr("Wheel LED"));
  QVBoxLayout *wheel_box_layout = new QVBoxLayout;
  wheel_box_layout->addWidget(m_wheel_led_widget);
  wheel_led_box->setLayout(wheel_box_layout);

  QGroupBox *side_led_box = new QGroupBox(this);
  side_led_box->setTitle(tr("Side LED"));
  QVBoxLayout *side_box_layout = new QVBoxLayout;
  side_box_layout->addWidget(m_side_led_widget);
  side_led_box->setLayout(side_box_layout);

  QHBoxLayout *led_layout = new QHBoxLayout;
  led_layout->addWidget(wheel_led_box);
  led_layout->addWidget(side_led_box);

  QFormLayout *dpi_layout = new QFormLayout;
  dpi_layout->addRow(tr("DPI 1 :"), m_dpi1_widget);
  dpi_layout->addRow(tr("DPI 2 :"), m_dpi2_widget);
  dpi_layout->addRow(tr("DPI 3 :"), m_dpi3_widget);
  dpi_layout->addRow(tr("DPI Instant Aim :"), m_dpiia_widget);

  QHBoxLayout *lift_distance_layout = new QHBoxLayout;
  lift_distance_layout->addWidget(new QLabel(tr("Lift off distance : ")
        ,this));
  lift_distance_layout->addWidget(m_lift_off_distance_slider);

  QFormLayout *buttons_layout_1 = new QFormLayout;
  QFormLayout *buttons_layout_2 = new QFormLayout;

  buttons_layout_1->addRow(tr("Left button :"),
      m_button_widget[MouseProfile::LeftButton]);
  buttons_layout_1->addRow(tr("Right button :"),
      m_button_widget[MouseProfile::RightButton]);
  buttons_layout_1->addRow(tr("Middle (wheel) button :"),
      m_button_widget[MouseProfile::WheelButton]);
  buttons_layout_1->addRow(tr("Wheel forward :"),
      m_button_widget[MouseProfile::WheelForward]);
  buttons_layout_1->addRow(tr("Wheel Backward :"),
      m_button_widget[MouseProfile::WheelBackward]);
  buttons_layout_1->addRow(tr("Side button back :"),
      m_button_widget[MouseProfile::SideButtonBack]);
  buttons_layout_2->addRow(tr("Side button front :"),
      m_button_widget[MouseProfile::SideButtonFront]);
  buttons_layout_2->addRow(tr("Instant Aim button :"),
      m_button_widget[MouseProfile::InstantAimButton]);
  buttons_layout_2->addRow(tr("Thumb button :"),
      m_button_widget[MouseProfile::ThumbButton]);
  buttons_layout_2->addRow(tr("Profile button :"),
      m_button_widget[MouseProfile::ProfileButton]);
  buttons_layout_2->addRow(tr("DPI front :"),
      m_button_widget[MouseProfile::DPIButtonFront]);
  buttons_layout_2->addRow(tr("DPI back :"),
      m_button_widget[MouseProfile::DPIButtonBack]);

  // TODO Make it align better

  QHBoxLayout *buttons_layout = new QHBoxLayout;
  buttons_layout->addLayout(buttons_layout_1);
  buttons_layout->addLayout(buttons_layout_2);

  QVBoxLayout *settings_layout = new QVBoxLayout;
  settings_layout->addLayout(led_layout);
  settings_layout->addLayout(dpi_layout);
  settings_layout->addStretch();
  settings_layout->addWidget(m_angle_snapping_check);
  settings_layout->addStretch();
  settings_layout->addLayout(lift_distance_layout);
  settings_layout->addStretch();

  QTabWidget *tab_widget = new QTabWidget(this);
  QWidget *settings_page_widget = new QWidget;
  QWidget *buttons_page_widget = new QWidget;

  settings_page_widget->setLayout(settings_layout);
  buttons_page_widget->setLayout(buttons_layout);

  tab_widget->addTab(settings_page_widget, tr("Settings"));
  tab_widget->addTab(buttons_page_widget, tr("Buttons settings"));

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(tab_widget);
  setLayout(layout);

  // Connections
  void (QSignalMapper:: *mapVoidSlot) (void) = &QSignalMapper::map;
  void (QSignalMapper:: *mappedIntSignal) (int) = &QSignalMapper::mapped;

  QSignalMapper *led_color_mapper = new QSignalMapper(this);
  led_color_mapper->setMapping(m_wheel_led_widget, MouseProfile::WheelLED);
  led_color_mapper->setMapping(m_side_led_widget, MouseProfile::SideLED);

  connect(m_wheel_led_widget, &LEDWidget::colorChanged,
      led_color_mapper, mapVoidSlot);
  connect(m_side_led_widget, &LEDWidget::colorChanged,
      led_color_mapper, mapVoidSlot);
  connect(led_color_mapper, mappedIntSignal,
      this, &ProfileWidget::updateLEDColor);

  QSignalMapper *led_state_mapper = new QSignalMapper(this);
  led_state_mapper->setMapping(m_wheel_led_widget, MouseProfile::WheelLED);
  led_state_mapper->setMapping(m_side_led_widget, MouseProfile::SideLED);

  connect(m_wheel_led_widget, &LEDWidget::stateChanged,
      led_state_mapper, mapVoidSlot);
  connect(m_side_led_widget, &LEDWidget::stateChanged,
      led_state_mapper, mapVoidSlot);
  connect(led_state_mapper, mappedIntSignal,
      this, &ProfileWidget::updateLEDState);

  m_dpi_x_mapper = new QSignalMapper(this);
  m_dpi_x_mapper->setMapping(m_dpi1_widget, MouseProfile::DPIOneX);
  m_dpi_x_mapper->setMapping(m_dpi2_widget, MouseProfile::DPITwoX);
  m_dpi_x_mapper->setMapping(m_dpi3_widget, MouseProfile::DPIThreeX);
  m_dpi_x_mapper->setMapping(m_dpiia_widget, MouseProfile::DPIInstantAimX);

  m_dpi_y_mapper = new QSignalMapper(this);
  m_dpi_y_mapper->setMapping(m_dpi1_widget, MouseProfile::DPIOneY);
  m_dpi_y_mapper->setMapping(m_dpi2_widget, MouseProfile::DPITwoY);
  m_dpi_y_mapper->setMapping(m_dpi3_widget, MouseProfile::DPIThreeY);
  m_dpi_y_mapper->setMapping(m_dpiia_widget, MouseProfile::DPIInstantAimY);

  connect(m_dpi1_widget, &DPIWidget::XDPIChanged,
      m_dpi_x_mapper, mapVoidSlot);
  connect(m_dpi2_widget, &DPIWidget::XDPIChanged,
      m_dpi_x_mapper, mapVoidSlot);
  connect(m_dpi3_widget, &DPIWidget::XDPIChanged,
      m_dpi_x_mapper, mapVoidSlot);
  connect(m_dpiia_widget, &DPIWidget::XDPIChanged,
      m_dpi_x_mapper, mapVoidSlot);

  connect(m_dpi_x_mapper, mappedIntSignal,
      this, &ProfileWidget::updateXDPI);

  connect(m_dpi1_widget, &DPIWidget::YDPIChanged,
      m_dpi_y_mapper, mapVoidSlot);
  connect(m_dpi2_widget, &DPIWidget::YDPIChanged,
      m_dpi_y_mapper, mapVoidSlot);
  connect(m_dpi3_widget, &DPIWidget::YDPIChanged,
      m_dpi_y_mapper, mapVoidSlot);
  connect(m_dpiia_widget, &DPIWidget::YDPIChanged,
      m_dpi_y_mapper, mapVoidSlot);

  connect(m_dpi_y_mapper, mappedIntSignal,
      this, &ProfileWidget::updateYDPI);

  connect(m_angle_snapping_check, &QCheckBox::toggled,
      this, &ProfileWidget::updateAngleSnapping);

  connect(m_lift_off_distance_slider, &QSlider::valueChanged,
      this, &ProfileWidget::updateLiftOffDistance);

  QSignalMapper *button_mapper = new QSignalMapper(this);
  for (int i=0;i<BUTTON_COUNT;++i)
  {
    button_mapper->setMapping(m_button_widget[i], i);
    connect(m_button_widget[i], &ButtonSettingWidget::valueChanged,
        button_mapper, mapVoidSlot);
  }
  connect(button_mapper, mappedIntSignal,
      this, &ProfileWidget::updateButtonSetting);
}

std::shared_ptr<MouseProfile> ProfileWidget::getProfile()
{
  return m_profile;
}

void ProfileWidget::setProfile(std::shared_ptr<MouseProfile> profile)
{
  m_profile = profile;
  setWidgets();
}

void ProfileWidget::setFilename(const QString &filename)
{
  m_filename = filename;
}

QString ProfileWidget::getFilename() const
{
  return m_filename;
}

QIcon ProfileWidget::getIcon() const
{
  return m_icon;
}

void ProfileWidget::updateLEDColor(int led)
{
  switch(led)
   {
    case MouseProfile::WheelLED:
      m_profile->setLEDColor(MouseProfile::WheelLED,
          m_wheel_led_widget->getColor().rgb());
      break;
    case MouseProfile::SideLED:
      m_profile->setLEDColor(MouseProfile::SideLED,
          m_side_led_widget->getColor().rgb());
      break;
    default:
      return;
  }
  drawIcon();
  emit profileModified();
}

void ProfileWidget::updateLEDState(int led)
{
  switch(led)
  {
    case MouseProfile::WheelLED:
      m_profile->setLEDState(MouseProfile::WheelLED,
          m_wheel_led_widget->getState());
      break;
    case MouseProfile::SideLED:
      m_profile->setLEDState(MouseProfile::SideLED,
          m_side_led_widget->getState());
      break;
    default:
      return;
  }
  emit profileModified();
}

void ProfileWidget::updateXDPI(int dpi_label)
{
  m_profile->setDPI(static_cast<MouseProfile::DPI>(dpi_label),
      static_cast<DPIWidget*>(
        m_dpi_x_mapper->mapping(dpi_label))->getXDPI());
  emit profileModified();
}

void ProfileWidget::updateYDPI(int dpi_label)
{
  m_profile->setDPI(static_cast<MouseProfile::DPI>(dpi_label),
      static_cast<DPIWidget*>(
        m_dpi_y_mapper->mapping(dpi_label))->getYDPI());
  emit profileModified();
}

void ProfileWidget::updateAngleSnapping(bool is_checked)
{
  m_profile->setAngleSnapping(is_checked);
  emit profileModified();
}

void ProfileWidget::updateLiftOffDistance(int value)
{
  m_profile->setLiftOffDistance(value);
  emit profileModified();
}

void ProfileWidget::updateButtonSetting(int button)
{
  m_profile->setButton(static_cast<MouseProfile::MouseButton>(button),
      m_button_widget[button]->getButtonSetting());
  emit profileModified();
}

void ProfileWidget::setWidgets()
{
  // LED
  m_wheel_led_widget->setColor(
      QColor::fromRgb(m_profile->getLEDColor(MouseProfile::WheelLED)));
  m_side_led_widget->setColor(
      QColor::fromRgb(m_profile->getLEDColor(MouseProfile::SideLED)));
  m_wheel_led_widget->setState(
      m_profile->getLEDState(MouseProfile::WheelLED));
  m_side_led_widget->setState(
      m_profile->getLEDState(MouseProfile::SideLED));
  // DPI
  m_dpi1_widget->setXDPI(m_profile->getDPI(MouseProfile::DPIOneX));
  m_dpi1_widget->setYDPI(m_profile->getDPI(MouseProfile::DPIOneY));
  m_dpi2_widget->setXDPI(m_profile->getDPI(MouseProfile::DPITwoX));
  m_dpi2_widget->setYDPI(m_profile->getDPI(MouseProfile::DPITwoY));
  m_dpi3_widget->setXDPI(m_profile->getDPI(MouseProfile::DPIThreeX));
  m_dpi3_widget->setYDPI(m_profile->getDPI(MouseProfile::DPIThreeY));
  m_dpiia_widget->setXDPI(m_profile->getDPI(MouseProfile::DPIInstantAimX));
  m_dpiia_widget->setYDPI(m_profile->getDPI(MouseProfile::DPIInstantAimY));

  // Angle snapping
  m_angle_snapping_check->setChecked(m_profile->getAngleSnapping());

  // Lift off distance
  m_lift_off_distance_slider->setValue(m_profile->getLiftOffDistance());

  //Buttons
  for (int i=0;i<BUTTON_COUNT;i++)
  {
    m_button_widget[i]->setButtonSetting(
        m_profile->getButtonSetting(static_cast<MouseProfile::MouseButton>(i)));
  }
}

void ProfileWidget::drawIcon()
{
  QPixmap icon_pic(100,100);
  icon_pic.fill(); //default color (currently Qt::White)
  QPainter painter(&icon_pic);
  QPoint a(0,25);
  QPoint b(75,100);
  QPoint side_triangle[3] = { a, QPoint(0,100), b};
  QPoint wheel_poly[5] = {a, QPoint(0,0), QPoint(100,0),QPoint(100,100), b};
  QBrush side_brush(QColor::fromRgb(
        m_profile->getLEDColor(MouseProfile::SideLED)));
  QBrush wheel_brush(QColor::fromRgb(
        m_profile->getLEDColor(MouseProfile::WheelLED)));
  QPen pen;
  pen.setWidth(5);

  painter.setPen(pen);
  painter.setBrush(side_brush);
  painter.drawPolygon(side_triangle, 3);
  painter.setBrush(wheel_brush);
  painter.drawPolygon(wheel_poly, 5);
  m_icon = QIcon(icon_pic);
  emit iconChanged();
}
