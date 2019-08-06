#include "mainwindow.h"
#include <QApplication>
#include "arduinouno.h"
#include "clickablelabel.h"
#include "../test/test.ino"

void loop();
void setup();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ArduinoUno::getInstance().set_loop(loop);
    setup();
    MainWindow w;
    w.show();

    return a.exec();
}
