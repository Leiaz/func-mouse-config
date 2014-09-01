#include "macro_input_widget.h"

#include "button_setting.h"

#include <QEvent>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPushButton>
#include <QString>

MacroInputWidget::MacroInputWidget(QWidget *parent)
  : QWidget(parent)
{
  m_display_edit = new QLineEdit(this);
  m_display_edit->setReadOnly(true);

  m_macro_input_button = new QPushButton(tr("Start/Stop"),this);
  m_macro_input_button->setCheckable(true);

  m_clear_button = new QPushButton(tr("Clear"));

  QGridLayout *layout = new QGridLayout;

  layout->addWidget(new QLabel(tr("Sequence (hexa):")),0,0);
  layout->addWidget(m_macro_input_button, 0,1);
  layout->addWidget(m_display_edit,1,0);
  layout->addWidget(m_clear_button,1,1);

  setLayout(layout);

  connect(m_clear_button, &QPushButton::clicked,
      this, &MacroInputWidget::clearMacro);
  connect(m_macro_input_button, &QPushButton::toggled,
      this, &MacroInputWidget::toggleMacroInput);
}

std::vector<unsigned char> MacroInputWidget::getMacro()
{
  return m_macro;
}

void MacroInputWidget::setMacro(std::vector<unsigned char> value)
{
  m_macro = value;
  QString text;
  for (const auto &elem: m_macro)
  {
    text.append(QString::number(elem, 16));
  }
  m_display_edit->setText(text);
}

void MacroInputWidget::mousePressEvent(QMouseEvent *event)
{
  if (m_macro_input_button->isChecked())
  {
    switch (event->button())
    {
      case Qt::LeftButton:
        addMacroItem(0x00);
        addMacroItem(0xf0);
        event->accept();
        break;
      case Qt::RightButton:
        addMacroItem(0x00);
        addMacroItem(0xf1);
        event->accept();
        break;
      case Qt::MiddleButton:
        addMacroItem(0x00);
        addMacroItem(0xf2);
        event->accept();
        break;
      default:
        QWidget::mousePressEvent(event);
    }
  } else {
    QWidget::mousePressEvent(event);
  }
}

void MacroInputWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (m_macro_input_button->isChecked())
  {
    switch (event->button())
    {
      case Qt::LeftButton:
        addMacroItem(0x80);
        addMacroItem(0xf0);
        event->accept();
        break;
      case Qt::RightButton:
        addMacroItem(0x80);
        addMacroItem(0xf1);
        event->accept();
        break;
      case Qt::MiddleButton:
        addMacroItem(0x80);
        addMacroItem(0xf2);
        event->accept();
        break;
      default:
        QWidget::mouseReleaseEvent(event);
    }
  } else {
    QWidget::mouseReleaseEvent(event);
  }
}

bool MacroInputWidget::event(QEvent *event)
{
  if (((event->type() == QEvent::KeyPress) ||
        (event->type() == QEvent::KeyRelease))
      && m_macro_input_button->isChecked())
  {
    QKeyEvent *key_event = static_cast<QKeyEvent*>(event);
    int xkeycode = key_event->nativeScanCode();
    if (event->type() == QEvent::KeyPress)
    {
      addMacroItem(0x00);
    }
    else
    {
      addMacroItem(0x80);
    }
    addMacroItem(ButtonSetting::getHIDFromXKeycode(xkeycode));
    event->accept();
    return true;
  }

  if ((event->type() == QEvent::ShortcutOverride) &&
      m_macro_input_button->isChecked())
  {
    event->accept();
    return true;
  }

  if ((event->type() == QEvent::FocusOut)
      && (!m_macro_input_button->hasFocus()))
  {
    m_macro_input_button->toggle();
  }
  return QWidget::event(event);
}

void MacroInputWidget::clearMacro()
{
  m_macro.clear();
  m_display_edit->clear();
  emit valueChanged();
}

void MacroInputWidget::toggleMacroInput()
{
  if (m_macro_input_button->isChecked())
  {
    setFocus();
    m_display_edit->setEnabled(false);
  }
  else
  {
    m_display_edit->setEnabled(true);
    emit valueChanged();
  }
}

void MacroInputWidget::addMacroItem(unsigned char item)
{
  if (m_macro.size() >= 512) return;
  m_macro.push_back(item);
  m_display_edit->setText(m_display_edit->text().append(
        QString::number(item, 16)));
}
