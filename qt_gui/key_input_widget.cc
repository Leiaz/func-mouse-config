#include "key_input_widget.h"

#include "button_setting.h"

#include <QEvent>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

KeyInputWidget::KeyInputWidget(QWidget *parent)
  : QWidget(parent)
{
  m_grab_key_button = new QPushButton(tr("Grab key"), this);
  m_grab_key_button->setCheckable(true);
  m_display_hid_edit = new QLineEdit(this);
  m_display_hid_edit->setInputMask("hH");
  m_display_hid_edit->setPlaceholderText("0");
  m_display_hid_edit->setReadOnly(false);

  QGridLayout *layout = new QGridLayout;

  layout->addWidget(new QLabel(tr("HID Usage ID (hexa):")),0,0);
  layout->addWidget(m_grab_key_button,1,0);
  layout->addWidget(m_display_hid_edit,1,1);

  setLayout(layout);

  connect(m_grab_key_button, &QPushButton::toggled,
      this, &KeyInputWidget::toggleGrabKey);
  connect(m_display_hid_edit, &QLineEdit::editingFinished,
      this, &KeyInputWidget::acceptEdit);
}

unsigned char KeyInputWidget::getHIDUsageID()
{
  return m_hid_usage_id;
}

void KeyInputWidget::setHIDUsageID(unsigned char value)
{
  m_hid_usage_id = value;
  m_display_hid_edit->setText(QString::number(m_hid_usage_id, 16));
  emit valueChanged(m_hid_usage_id);
}

bool KeyInputWidget::event(QEvent *event)
{
  if ((event->type() == QEvent::KeyPress) &&
      (m_grab_key_button->isChecked()))
  {
    QKeyEvent *key_event = static_cast<QKeyEvent*>(event);
    int xkeycode = key_event->nativeScanCode();
    setHIDUsageID(ButtonSetting::getHIDFromXKeycode(xkeycode));
    m_grab_key_button->toggle();
    event->accept();
    return true;
  }

  if ((event->type() == QEvent::FocusOut)
      && (!m_grab_key_button->hasFocus()))
  {
    m_grab_key_button->toggle();
  }

  return QWidget::event(event);
}

void KeyInputWidget::toggleGrabKey()
{
  if (m_grab_key_button->isChecked())
  {
    m_display_hid_edit->setReadOnly(true);
    setFocus();
  } else {
    m_display_hid_edit->setReadOnly(false);
    m_display_hid_edit->setFocus();
  }
}

void KeyInputWidget::acceptEdit()
{
  setHIDUsageID(m_display_hid_edit->text().toInt(0, 16));
}
