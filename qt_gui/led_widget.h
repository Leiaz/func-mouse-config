#ifndef LED_WIDGET_H
#define LED_WIDGET_H

#include "mouse_profile.h"

#include <QColor>
#include <QWidget>

class ColorButton;
class QComboBox;

class LEDWidget : public QWidget
{
  Q_OBJECT

  public:
    LEDWidget(QWidget *parent = 0);

    void setColor(QColor color);
    QColor getColor() const;
    void setState(MouseProfile::LEDState state);
    MouseProfile::LEDState getState() const;

  signals:
    void colorChanged(QColor new_color);
    void stateChanged(MouseProfile::LEDState state);

  private slots:
    void colorButtonPushed();
    void stateIndexChanged(int index);

  private:
    QColor m_color;
    MouseProfile::LEDState m_state = MouseProfile::LEDOff;

    ColorButton *m_color_button;
    QComboBox *m_state_combo;
};
#endif
