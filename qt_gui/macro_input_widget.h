#ifndef MACRO_INPUT_WIDGET_H
#define MACRO_INPUT_WIDGET_H

#include <QWidget>

#include <vector>

class QLineEdit;
class QPushButton;

class MacroInputWidget : public QWidget
{
  Q_OBJECT

  public:
    MacroInputWidget(QWidget *parent = 0);

    std::vector<unsigned char> getMacro();
    void setMacro(std::vector<unsigned char> value);

  signals:
    void valueChanged();

  protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool event(QEvent *event);

  private slots:
    void clearMacro();
    void toggleMacroInput();

  private:
    void addMacroItem(unsigned char item);

    std::vector<unsigned char> m_macro;

    QPushButton *m_macro_input_button;
    QPushButton *m_clear_button;
    QLineEdit *m_display_edit;
};
#endif
