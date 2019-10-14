#include "clickablelineedit.h"
#include "arduinouno.h"
#include <QInputDialog>
#include <QLineEdit>

ClickableLineEdit::ClickableLineEdit(QWidget *parent, Qt::WindowFlags f)
    :QLineEdit (parent)
{

}

ClickableLineEdit::ClickableLineEdit(const QString &text, QWidget *parent)
    :QLineEdit (parent)
{
    this->setText(text);
}

ClickableLineEdit::~ClickableLineEdit()
{

}

void ClickableLineEdit::setI(int i_)
{
    i = i_;
}

int ClickableLineEdit::getI()
{
    return i;
}

void ClickableLineEdit::set_analog_pin()
{
    bool ok;
    QString text = QInputDialog::getText(this, trUtf8("Установить значение аналогового пина"),
                                             trUtf8("Введите значение"), QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty())
           ArduinoUno::getInstance().analog_write(i,text.toInt());
}

void ClickableLineEdit::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked();
}
