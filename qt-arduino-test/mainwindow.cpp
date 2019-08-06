#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "arduinouno.h"
#include <QList>
#include <QDebug>
#include "clickablelineedit.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer.setInterval(1);
    timer_work.setInterval(2);
    connect(&timer,SIGNAL(timeout()),this,SLOT(show_state()));
    connect(&timer_work,SIGNAL(timeout()),this,SLOT(loop_state()));

    QList<QWidget*> mylist= findChildren<QWidget*>();
    foreach (QWidget* current, mylist) {
        ClickableLabel *label_ = qobject_cast<ClickableLabel *>(current);
        ClickableLineEdit *line_ = qobject_cast<ClickableLineEdit *>(current);
        if(label_){
            QVariant digital_pin = label_->property("digital");
            if (digital_pin.isValid()) {
              label_->setI(digital_pin.toInt());
              digital_pin_labels.insert(digital_pin.toInt(),label_);
              connect(label_,SIGNAL(clicked()),label_,SLOT(set_digital_pin()));
            }
        }
        if(line_){
            QVariant analog_pin = line_->property("analog");
            if (analog_pin.isValid()) {
              line_->setI(analog_pin.toInt());
              analog_pin_line_edit.insert(analog_pin.toInt(),line_);
              connect(line_,SIGNAL(clicked()),line_,SLOT(set_analog_pin()));
            }
        }
    }
    timer.start();
    timer_work.start();
}

MainWindow::~MainWindow()
{
    timer.stop();
    timer_work.stop();
    delete ui;
}

void MainWindow::show_state()
{
 //   if(ArduinoUno)
    for(int i = 0; i < digital_pin_labels.count(); i++){
        if(ArduinoUno::getInstance().digital_read(digital_pin_labels.at(i)->getI())){
            digital_pin_labels.at(i)->setStyleSheet("QLabel { background-color : green; color : green; }");
        }else {
            digital_pin_labels.at(i)->setStyleSheet("QLabel { background-color : white; color : white; }");
        }
    }
    for(int i = 0; i < analog_pin_line_edit.count(); i++){
        analog_pin_line_edit.at(i)->setText(QString::number(
                                                ArduinoUno::getInstance().analog_read(
                                                    analog_pin_line_edit.at(i)->getI())));
    }
}

void MainWindow::loop_state()
{
   ArduinoUno::getInstance().get_loop();
}


void MainWindow::on_pushButton_clicked()
{
    ArduinoUno::getInstance().reset();
}

void MainWindow::on_pushButton_2_clicked()
{
    ArduinoUno::getInstance().get_loop();
}
