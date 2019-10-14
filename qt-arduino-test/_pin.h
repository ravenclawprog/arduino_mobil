#ifndef _PIN_H
#define _PIN_H

#include "arduino_interfaces.h"
class _pin
{

public:
    enum type_of_pin{
        digital_type = 0,
        analog_type
    };
    enum mode_of_pin{
        output_mode = 0,
        input_mode,
        input_pullup_mode
    };

    _pin(type_of_pin t = _pin::digital_type,
         mode_of_pin m = _pin::output_mode,
         bool digital_state = LOW,
         int  analog_state  = 0);
    bool get_digital_state();
    int  get_analog_state();
    void set_digital_state(bool state);
    void set_analog_state(int state);
    type_of_pin get_type_of_pin();
    mode_of_pin get_mode_of_pin();
private:
    type_of_pin t_;
    mode_of_pin m_;
    bool digital_state_;
    int  analog_state_;
};

#endif // _PIN_H
