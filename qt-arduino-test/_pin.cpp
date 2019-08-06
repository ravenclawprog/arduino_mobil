#include "_pin.h"


_pin::_pin(_pin::type_of_pin t, _pin::mode_of_pin m, bool digital_state, int analog_state)
    : t_(t),
      m_(m),
      digital_state_(digital_state),
      analog_state_(analog_state)
{

}

bool _pin::get_digital_state()
{
    return digital_state_;
}

int _pin::get_analog_state()
{
    return  analog_state_;
}

void _pin::set_digital_state(bool state)
{
    digital_state_ = state;
}

void _pin::set_analog_state(int state)
{
    analog_state_ = state;
}

_pin::type_of_pin _pin::get_type_of_pin()
{
    return t_;
}

_pin::mode_of_pin _pin::get_mode_of_pin()
{
    return  m_;
}
