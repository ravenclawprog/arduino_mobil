#include "clickablelabel.h"
#include "arduinouno.h"

ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f):QLabel(parent)
{

}

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent)
    : QLabel(parent)
{
    setText(text);
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::setI(int i_)
{
    i = i_;
}

int ClickableLabel::getI()
{
    return  i;
}

void ClickableLabel::set_digital_pin()
{
    ArduinoUno::getInstance().digital_write(i,
  !(ArduinoUno::getInstance().digital_read(i)));
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit clicked();
}
