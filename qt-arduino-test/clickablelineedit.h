#ifndef CLICKABLELINEEDIT_H
#define CLICKABLELINEEDIT_H

#include "QLineEdit"
class ClickableLineEdit : public QLineEdit
{
Q_OBJECT
public:
    explicit ClickableLineEdit(QWidget *parent=0, Qt::WindowFlags f=0);
    explicit ClickableLineEdit( const QString& text="", QWidget* parent=0 );
    ~ClickableLineEdit();
    void setI(int i_);
    int  getI();
signals:
    void clicked();
public slots:
    void set_analog_pin();
private:
    int i;
protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // CLICKABLELINEEDIT_H
