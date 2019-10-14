#include "arduinomachine.h"
#include <QTimer>
#include <QTime>
#include <QDebug>
ArduinoMachine::ArduinoMachine()
{
    reset();
}

bool ArduinoMachine::digital_read(int pin)
{
    if(pin >= 0 && pin < d_pins_max){
        return digital_pins[pin].read();
    } else {
        return false;
    }
}

void ArduinoMachine::digital_write(int pin, bool state_)
{
    if(pin >= 0 && pin < d_pins_max){
      digital_pins[pin].write(state_);
    }
}

int ArduinoMachine::analog_read(int pin)
{
    if(pin >= 0 && pin < a_pins_max){
      return analog_pins[pin].read();
    } else {
        return  0;
    }
}

void ArduinoMachine::analog_write(int pin, int state_)
{
    if(pin >= 0 && pin < a_pins_max){
      analog_pins[pin].write(state_);
    }
}

unsigned long ArduinoMachine::millis()
{
    return static_cast<unsigned long>(QDateTime::currentDateTime().currentMSecsSinceEpoch());
    //return counter_timer0_;
}

void ArduinoMachine::reset()
{
    on_ = true;
    counter_timer0_ = 0;
    for(int i = 0; i < d_pins_max; i++){
        digital_pins[i].write(LOW);
    }
    for(int i = 0; i < a_pins_max; i++){
        analog_pins[i].write(0);
    }
}

void ArduinoMachine::set_loop(void (*loop)())
{
    loop_ = loop;
}

void ArduinoMachine::get_loop()
{
    (*loop_)();
}

void ArduinoMachine::timer0_count()
{
    //counter_timer0_ ++;

}
