#ifndef ARDUINOMACHINE_H
#define ARDUINOMACHINE_H

#include "__pin.h"
#include "arduino_interfaces.h"
#include <QTimer>

class ArduinoMachine
{
public:
    ArduinoMachine();
    bool digital_read(int pin);
    void digital_write(int pin, bool state_);
    int analog_read(int pin);
    void analog_write(int pin, int state_);
    unsigned long millis();
    void reset();
    void set_loop(void (*loop)());
    void get_loop();
private:
    __pin<bool> digital_pins[d_pins_max];
    __pin<int>  analog_pins[a_pins_max];
    bool on_;
    volatile unsigned long counter_timer0_;
    void timer0_count();
    void (*loop_)();
};


#endif // ARDUINOMACHINE_H
