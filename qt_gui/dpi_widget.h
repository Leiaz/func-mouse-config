#ifndef DPI_WIDGET_H
#define DPI_WIDGET_H

#include <QWidget>

class QCheckBox;
class QLabel;
class QSlider;
class QSpinBox;

class DPIWidget : public QWidget
{
  Q_OBJECT

  public:
    /**
     * \param dpi_min_step : Minimum DPI value in number of steps.
     * \param dpi_max_step : Maximum DPI value in number of steps.
     * \param dpi_step_size : the actual value of one step in DPI.
     */
    DPIWidget(unsigned char dpi_min_step,
        unsigned char dpi_max_step, int dpi_step_size,
        QWidget *parent = 0);
    unsigned char getYDPI();
    unsigned char getXDPI();

  public slots:
    void setYDPI(int value);
    void setXDPI(int value);

  signals:
    void XDPIChanged(int value);
    void YDPIChanged(int value);

  private slots:
    void sameXYEnabled(bool enabled);

  private:
    bool m_same_xy = false;
    unsigned char m_x_dpi_steps = 0;
    unsigned char m_y_dpi_steps = 0;
    unsigned char m_dpi_min_step;
    unsigned char m_dpi_max_step;
    int m_dpi_step_value;

    QSlider *m_x_dpi_slider;
    QSlider *m_y_dpi_slider;
    QSpinBox *m_x_dpi_edit;
    QSpinBox *m_y_dpi_edit;
    QLabel *m_x_dpi_label;
    QLabel *m_y_dpi_label;
    QCheckBox *m_same_xy_check;
};
#endif
