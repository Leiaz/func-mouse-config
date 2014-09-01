#include "led_widget.h"

#include <QColorDialog>
#include <QFormLayout>
#include <QComboBox>

#include "color_button.h"

LEDWidget::LEDWidget(QWidget *parent):
  QWidget(parent)
{

  m_state_combo = new QComboBox(this);
  m_state_combo->addItem(tr("Off"), MouseProfile::LEDOff);
  m_state_combo->addItem(tr("On"), MouseProfile::LEDOn);
  m_state_combo->addItem(tr("Pulsate"), MouseProfile::LEDPulsate);
  m_color_button = new ColorButton(this);

  connect(m_color_button, &QPushButton::clicked,
      this, &LEDWidget::colorButtonPushed);
  connect(m_state_combo,
      (void (QComboBox:: *) (int)) &QComboBox::currentIndexChanged,
      this, &LEDWidget::stateIndexChanged);

  QFormLayout *layout = new QFormLayout(this);

  layout->addRow(tr("LED state :"), m_state_combo);
  layout->addRow(tr("LED color :"), m_color_button);

  setLayout(layout);
}

void LEDWidget::setColor(QColor color)
{
  m_color = color;
  m_color_button->setColor(color);
  emit colorChanged(m_color);
}

QColor LEDWidget::getColor() const
{
  return m_color;
}

void LEDWidget::setState(MouseProfile::LEDState state)
{
  m_state = state;
  m_state_combo->setCurrentIndex(m_state_combo->findData(m_state));
  if (m_state == MouseProfile::LEDOff)
  {
    m_color_button->setEnabled(false);
  } else {
    m_color_button->setEnabled(true);
  }
  emit stateChanged(m_state);
}

MouseProfile::LEDState LEDWidget::getState() const
{
  return m_state;
}

/*
 */
void LEDWidget::colorButtonPushed()
{
  QColor c = QColorDialog::getColor(m_color, this,
      tr("LED color"));
  if (c.isValid())
  {
    setColor(c);
  }
}

void LEDWidget::stateIndexChanged(int index)
{
  setState(static_cast<MouseProfile::LEDState>
      (m_state_combo->itemData(index).toInt()));
}
