#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

class ClickableLabel : public QLabel
{
Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent=0, Qt::WindowFlags f=0);
    explicit ClickableLabel( const QString& text="", QWidget* parent=0 );
    ~ClickableLabel();
    void setI(int i_);
    int  getI();
signals:
    void clicked();
public slots:
    void set_digital_pin();
private:
    int i;
protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // CLICKABLELABEL_H
