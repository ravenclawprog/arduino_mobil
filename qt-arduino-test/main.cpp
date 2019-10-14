#include "mainwindow.h"
#include <QApplication>
#include "arduinouno.h"
#include "clickablelabel.h"
#include "../arduino_mobil.ino"

extern void loop();
extern void setup();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ArduinoUno::getInstance().set_loop(loop);
    setup();
    MainWindow w;
    w.show();

    return a.exec();
}
