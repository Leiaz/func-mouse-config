#ifndef BUTTON_SETTING_WIDGET_H
#define BUTTON_SETTING_WIDGET_H

#include "button_setting.h"

#include <QWidget>

class KeyInputWidget;
class MacroInputWidget;
class QComboBox;
class QStackedLayout;

class ButtonSettingWidget : public QWidget
{
  Q_OBJECT

  public:
    ButtonSettingWidget(QWidget *parent = 0);
    ButtonSetting getButtonSetting() const;

  signals:
    void valueChanged();

  public slots:
    void setButtonSetting(ButtonSetting setting);

  private slots:
    void acceptChanges();

  private:
    ButtonSetting m_button_setting;

    QComboBox *m_setting_type_combo;
    QComboBox *m_mouse_action_combo;
    QComboBox *m_media_button_combo;
    QComboBox *m_shortcut_combo;
    KeyInputWidget *m_single_key_input;
    MacroInputWidget *m_macro_input;
    QStackedLayout *m_setting_stack;
};
#endif
