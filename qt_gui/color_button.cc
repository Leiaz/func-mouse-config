#include "color_button.h"

#include <QPainter>

ColorButton::ColorButton(QWidget *parent):
  QPushButton(parent),m_current_color(Qt::black)
{
}

QColor ColorButton::getColor() const
{
  return m_current_color;
}

void ColorButton::setColor(QColor c)
{
  m_current_color = c;
  update();
}

void ColorButton::paintEvent(QPaintEvent *event)
{
  QPushButton::paintEvent(event);
  if (isEnabled())
  {
    QPainter painter(this);
    double border = height()*0.2;
    painter.setBrush(m_current_color);
    painter.drawRect(border,border,width()-border*2.0,height()-border*2.0);
  }
}
