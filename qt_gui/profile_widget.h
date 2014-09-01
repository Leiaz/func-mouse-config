#ifndef PROFILE_WIDGET_H
#define PROFILE_WIDGET_H

#include "mouse_profile.h"

#include <QIcon>
#include <QWidget>

class ButtonSettingWidget;
class DPIWidget;
class LEDWidget;
class QCheckBox;
class QSignalMapper;
class QSlider;

class ProfileWidget : public QWidget
{
  Q_OBJECT

  public:
    ProfileWidget(std::shared_ptr<MouseProfile> p = nullptr,
        QWidget *parent = 0);

    std::shared_ptr<MouseProfile> getProfile();
    void setProfile(std::shared_ptr<MouseProfile> profile);
    void setFilename(const QString &filename);
    QString getFilename() const;
    bool isModified() const;
    void resetModified();
    QIcon getIcon() const;

  signals:
    void iconChanged();
    void profileModified();

  private slots:
    /**
     * \param led is MouseProfile::LED enum
     */
    void updateLEDColor(int led);
    void updateLEDState(int led);
    /**
     * \param dpi_label is a MouseProfile::DPI enum
     * XXX And the value will be asked to the sender
     */
    void updateXDPI(int dpi_label);
    void updateYDPI(int dpi_label);
    void updateAngleSnapping(bool isChecked);
    void updateLiftOffDistance(int value);
    void updateButtonSetting(int button);

  private:
    void setWidgets();
    void drawIcon();

    std::shared_ptr<MouseProfile> m_profile;
    QString m_filename;
    QIcon m_icon;

    LEDWidget *m_wheel_led_widget;
    LEDWidget *m_side_led_widget;
    DPIWidget *m_dpi1_widget;
    DPIWidget *m_dpi2_widget;
    DPIWidget *m_dpi3_widget;
    DPIWidget *m_dpiia_widget;
    QCheckBox *m_angle_snapping_check;
    QSlider *m_lift_off_distance_slider;
    ButtonSettingWidget *m_button_widget[BUTTON_COUNT];

    QSignalMapper *m_dpi_x_mapper;
    QSignalMapper *m_dpi_y_mapper;
};
#endif
