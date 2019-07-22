#ifndef ZEPPELIN_PWM_H
#define ZEPPELIN_PWM_H

#include <Arduino.h>

class ZEPPELIN_PWM{
public:
    ZEPPELIN_PWM(int pin = A7, int status = 0);
    ~ZEPPELIN_PWM();
    operator int() const;
    int read();
    int getState();
private:
    int pin_;
    int status_;
    int prev_status_;
};



ZEPPELIN_PWM::ZEPPELIN_PWM(int pin, int status)
{
    pin_         = pin;
    status_      = status;
    prev_status_ = 0;
    pinMode(pin_, INPUT);
}

ZEPPELIN_PWM::~ZEPPELIN_PWM()
{
    pin_ = 0;
    status_ = 0;
    prev_status_ = 0;
}

int ZEPPELIN_PWM::read()
{
    prev_status_ = status_;
    status_ = analogRead(pin_);
}

int ZEPPELIN_PWM::getState()
{
    return status_;
}

ZEPPELIN_PWM::operator int() const
{
    return status_;
}

#endif // ZEPPELIN_PWM_H
