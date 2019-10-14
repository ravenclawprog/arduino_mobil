#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QList>
#include "clickablelabel.h"
#include "clickablelineedit.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer timer;// таймер для отображения
    QTimer timer_work;// таймер для работы
    QList<ClickableLabel*> digital_pin_labels;
    QList<ClickableLineEdit*> analog_pin_line_edit;
private slots:
    void show_state();
    void loop_state();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
