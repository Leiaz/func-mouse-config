#include "button_setting_widget.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QVBoxLayout>

#include "key_input_widget.h"
#include "macro_input_widget.h"

ButtonSettingWidget::ButtonSettingWidget(QWidget *parent)
  :QWidget(parent)
{
  m_setting_type_combo = new QComboBox(this);
  m_setting_type_combo->addItem(tr("Mouse action"),
      ButtonSetting::MouseAction);
  m_setting_type_combo->addItem(tr("Media button"),
      ButtonSetting::MediaButton);
  m_setting_type_combo->addItem(tr("Shortcut"),
      ButtonSetting::CommonShortcut);
  m_setting_type_combo->addItem(tr("Single key"),
      ButtonSetting::SingleKey);
  m_setting_type_combo->addItem(tr("Macro"),
      ButtonSetting::Macro);
  m_setting_type_combo->addItem(tr("Disabled"),
      ButtonSetting::Disabled);

  m_mouse_action_combo = new QComboBox(this);
  m_mouse_action_combo->addItem(tr("Button 1 (left click)"),
      ACTION_BUTTON_1);
  m_mouse_action_combo->addItem(tr("Button 2 (right click)"),
      ACTION_BUTTON_2);
  m_mouse_action_combo->addItem(tr("Button 3 (middle click)"),
      ACTION_BUTTON_3);
  m_mouse_action_combo->addItem(tr("Forward"), ACTION_FORWARD);
  m_mouse_action_combo->addItem(tr("Backward"), ACTION_BACKWARD);
  m_mouse_action_combo->addItem(tr("Scroll up"), ACTION_SCROLL_UP);
  m_mouse_action_combo->addItem(tr("Scroll down"), ACTION_SCROLL_DOWN);
  m_mouse_action_combo->addItem(tr("DPI +"), ACTION_DPI_UP);
  m_mouse_action_combo->addItem(tr("DPI -"), ACTION_DPI_DOWN);
  m_mouse_action_combo->addItem(tr("Cycle DPI"), ACTION_DPI_CYCLE);
  m_mouse_action_combo->addItem(tr("Instant Aim"), ACTION_INSTANT_AIM);
  m_mouse_action_combo->addItem(tr("Cycle Profiles"),
      ACTION_PROFILE_CYCLE);
  m_mouse_action_combo->addItem(tr("Profile +"), ACTION_PROFILE_UP);
  m_mouse_action_combo->addItem(tr("Profile -"), ACTION_PROFILE_DOWN);

  m_media_button_combo = new QComboBox(this);
  m_media_button_combo->addItem(tr("Play/Pause"), MEDIA_PLAY_PAUSE);
  m_media_button_combo->addItem(tr("Volume Up"), MEDIA_VOLUME_UP);
  m_media_button_combo->addItem(tr("Volume Down"), MEDIA_VOLUME_DOWN);
  m_media_button_combo->addItem(tr("Mute"), MEDIA_MUTE);
  m_media_button_combo->addItem(tr("Next track"), MEDIA_NEXT_TRACK);
  m_media_button_combo->addItem(tr("Previous track"),
      MEDIA_PREVIOUS_TRACK);

  m_shortcut_combo = new QComboBox(this);
  m_shortcut_combo->addItem(tr("Ctrl+O (Open)"), SHORTCUT_CTRL_O);
  m_shortcut_combo->addItem(tr("Ctrl+C (Copy)"), SHORTCUT_CTRL_C);
  m_shortcut_combo->addItem(tr("Ctrl+V (Paste)"), SHORTCUT_CTRL_V);
  m_shortcut_combo->addItem(tr("Ctrl+X (Cut)"), SHORTCUT_CTRL_X);
  m_shortcut_combo->addItem(tr("Ctrl+Z (Undo)"), SHORTCUT_CTRL_Z);
  m_shortcut_combo->addItem(tr("Ctrl+Shift+Z (Redo)"),
      SHORTCUT_CTRL_SHIFT_Z);
  m_shortcut_combo->addItem(tr("Ctrl+A (Select All)"), SHORTCUT_CTRL_A);
  m_shortcut_combo->addItem(tr("Ctrl+F (Find)"), SHORTCUT_CTRL_F);
  m_shortcut_combo->addItem(tr("Ctrl+N (New)"), SHORTCUT_CTRL_N);

  m_single_key_input = new KeyInputWidget(this);

  m_macro_input = new MacroInputWidget(this);

  // Layout

  m_setting_stack = new QStackedLayout;
  m_setting_stack->addWidget(m_mouse_action_combo);
  m_setting_stack->addWidget(m_media_button_combo);
  m_setting_stack->addWidget(m_shortcut_combo);
  m_setting_stack->addWidget(m_single_key_input); // Single key
  m_setting_stack->addWidget(m_macro_input); //Macro
  m_setting_stack->addWidget(new QWidget); // Disabled (empty widget)

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(m_setting_type_combo);
  layout->addLayout(m_setting_stack);

  setLayout(layout);

  void (QComboBox:: *activatedIntSignal)(int) =
    &QComboBox::activated;
  connect(m_setting_type_combo,
      (void (QComboBox:: *)(int))&QComboBox::currentIndexChanged,
      m_setting_stack, &QStackedLayout::setCurrentIndex);
  connect(m_setting_type_combo, activatedIntSignal,
      this, &ButtonSettingWidget::acceptChanges);
  connect(m_mouse_action_combo, activatedIntSignal,
      this, &ButtonSettingWidget::acceptChanges);
  connect(m_media_button_combo, activatedIntSignal,
      this, &ButtonSettingWidget::acceptChanges);
  connect(m_shortcut_combo, activatedIntSignal,
      this, &ButtonSettingWidget::acceptChanges);
  connect(m_single_key_input, &KeyInputWidget::valueChanged,
      this, &ButtonSettingWidget::acceptChanges);
  connect(m_macro_input, &MacroInputWidget::valueChanged,
      this, &ButtonSettingWidget::acceptChanges);
}

ButtonSetting ButtonSettingWidget::getButtonSetting() const
{
  return m_button_setting;
}

void ButtonSettingWidget::setButtonSetting(ButtonSetting setting)
{
  m_button_setting = setting;
  ButtonSetting::Type type = m_button_setting.getType();
  m_setting_type_combo->setCurrentIndex(type);

  switch (type)
  {
    case ButtonSetting::MouseAction:
      m_mouse_action_combo->setCurrentIndex(
          m_mouse_action_combo->findData(m_button_setting.getValue()));
      break;
    case ButtonSetting::MediaButton:
      m_media_button_combo->setCurrentIndex(
          m_media_button_combo->findData(m_button_setting.getValue()));
      break;
    case ButtonSetting::CommonShortcut:
      m_shortcut_combo->setCurrentIndex(
          m_shortcut_combo->findData(m_button_setting.getValue()));
      break;
    case ButtonSetting::SingleKey:
      m_single_key_input->setHIDUsageID(m_button_setting.getValue());
      break;
    case ButtonSetting::Macro:
      m_macro_input->setMacro(m_button_setting.getMacro());
      break;
    case ButtonSetting::Disabled:
      break;
  }
}

void ButtonSettingWidget::acceptChanges()
{
  int type = m_setting_type_combo->currentIndex();

  switch(type)
  {
    case ButtonSetting::MouseAction:
      m_button_setting.setMouseAction(
          m_mouse_action_combo->currentData().toInt());
      break;
    case ButtonSetting::MediaButton:
      m_button_setting.setMediaButton(
          m_media_button_combo->currentData().toInt());
      break;
    case ButtonSetting::CommonShortcut:
      m_button_setting.setShortcut(
          m_shortcut_combo->currentData().toInt());
      break;
    case ButtonSetting::SingleKey:
      m_button_setting.setSingleKey(m_single_key_input->getHIDUsageID());
      break;
    case ButtonSetting::Macro:
      m_button_setting.setMacro(m_macro_input->getMacro());
      break;
    case ButtonSetting::Disabled:
      m_button_setting.setDisabled();
  }
  emit valueChanged();
}
