#include "dpi_widget.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

DPIWidget::DPIWidget(unsigned char dpi_min_step,
    unsigned char dpi_max_step, int dpi_step_size, QWidget *parent)
  : QWidget(parent),
  m_dpi_min_step(dpi_min_step),
  m_dpi_max_step(dpi_max_step),
  m_dpi_step_value(dpi_step_size)
{
  m_x_dpi_slider = new QSlider(Qt::Horizontal,this);
  m_x_dpi_slider->setMinimum(m_dpi_min_step);
  m_x_dpi_slider->setMaximum(m_dpi_max_step);
  m_y_dpi_slider = new QSlider(Qt::Horizontal,this);
  m_y_dpi_slider->setMinimum(m_dpi_min_step);
  m_y_dpi_slider->setMaximum(m_dpi_max_step);

  m_x_dpi_edit = new QSpinBox(this);
  m_x_dpi_edit->setReadOnly(true); //for now ?
  m_x_dpi_edit->setMinimum(m_dpi_min_step * m_dpi_step_value);
  m_x_dpi_edit->setMaximum(m_dpi_max_step * m_dpi_step_value);
  m_y_dpi_edit = new QSpinBox(this);
  m_y_dpi_edit->setReadOnly(true); //for now ?
  m_y_dpi_edit->setMinimum(m_dpi_min_step * m_dpi_step_value);
  m_y_dpi_edit->setMaximum(m_dpi_max_step * m_dpi_step_value);

  m_x_dpi_label = new QLabel(tr("X : "), this);
  m_y_dpi_label = new QLabel(tr("Y : "), this);

  m_same_xy_check = new QCheckBox(tr("Same X && Y values"), this);

  QVBoxLayout *layout = new QVBoxLayout;

  layout->addWidget(m_same_xy_check);

  QHBoxLayout *x_line_layout = new QHBoxLayout;
  x_line_layout->addWidget(m_x_dpi_label);
  x_line_layout->addWidget(m_x_dpi_slider);
  x_line_layout->addWidget(m_x_dpi_edit);

  QHBoxLayout *y_line_layout = new QHBoxLayout;
  y_line_layout->addWidget(m_y_dpi_label);
  y_line_layout->addWidget(m_y_dpi_slider);
  y_line_layout->addWidget(m_y_dpi_edit);

  layout->addLayout(x_line_layout);
  layout->addLayout(y_line_layout);

  setLayout(layout);

  connect(m_x_dpi_slider, &QSlider::valueChanged,
      this, &DPIWidget::setXDPI);
  connect(m_y_dpi_slider, &QSlider::valueChanged,
      this, &DPIWidget::setYDPI);

  connect(m_same_xy_check, &QCheckBox::toggled,
      this, &DPIWidget::sameXYEnabled);
}

unsigned char DPIWidget::getXDPI()
{
  return m_x_dpi_steps;
}

unsigned char DPIWidget::getYDPI()
{
  return m_y_dpi_steps;
}

void DPIWidget::setXDPI(int value)
{
  if ((value > m_dpi_max_step)||(value < m_dpi_min_step)
      ||(value==m_x_dpi_steps))
  {
    return;
  }

  m_x_dpi_steps = value;
  m_x_dpi_slider->setValue(value);
  m_x_dpi_edit->setValue(value * m_dpi_step_value);

  emit XDPIChanged(m_x_dpi_steps);
}

void DPIWidget::setYDPI(int value)
{
  if ((value > m_dpi_max_step)||(value < m_dpi_min_step)
      ||(value==m_y_dpi_steps))
  {
    return;
  }

  m_y_dpi_steps = value;
  m_y_dpi_slider->setValue(value);
  m_y_dpi_edit->setValue(value * m_dpi_step_value);

  emit YDPIChanged(m_y_dpi_steps);
}

/**
 *
 */
void DPIWidget::sameXYEnabled(bool enabled)
{
  if (enabled)
  {
    connect(this, &DPIWidget::XDPIChanged,
        this, &DPIWidget::setYDPI);
    m_y_dpi_slider->setEnabled(false);
    m_y_dpi_edit->setEnabled(false);
    m_y_dpi_label->setEnabled(false);
    setYDPI(getXDPI());
  } else {
    disconnect(this, &DPIWidget::XDPIChanged,
        this, &DPIWidget::setYDPI);
    m_y_dpi_slider->setEnabled(true);
    m_y_dpi_edit->setEnabled(true);
    m_y_dpi_label->setEnabled(true);
  }
}
