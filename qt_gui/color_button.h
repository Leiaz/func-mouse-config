#ifndef COLOR_BUTTON_H
#define COLOR_BUTTON_H

#include <QColor>
#include <QPushButton>

class ColorButton : public QPushButton
{
  Q_OBJECT

  public:
    ColorButton(QWidget *parent = 0);
    QColor getColor() const;

  public slots:
    void setColor(QColor c);

  protected:
    void paintEvent(QPaintEvent *event);

  private:
    QColor m_current_color;
};
#endif
