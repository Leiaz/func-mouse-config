#ifndef KEY_INPUT_WIDGET_H
#define KEY_INPUT_WIDGET_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class KeyInputWidget : public QWidget
{
  Q_OBJECT

  public:
    KeyInputWidget(QWidget *parent = 0);

    unsigned char getHIDUsageID();
    void setHIDUsageID(unsigned char value);

  signals:
    void valueChanged(unsigned char value);

  protected:
    bool event(QEvent * event);

  private slots:
    void toggleGrabKey();
    void acceptEdit();

  private:
    unsigned char m_hid_usage_id = 0;

    QPushButton *m_grab_key_button;
    QLineEdit *m_display_hid_edit;
};
#endif
