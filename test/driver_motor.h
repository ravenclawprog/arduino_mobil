#ifndef DRIVER_MOTOR_H
#define DRIVER_MOTOR_H

class driver_motor{
public:
    driver_motor(int pin_LPWM = 2,
                 int pin_RPWM = 4,
                 int pin_PWM  = 3,
                 bool LPWM_state = false,
                 bool RPWM_state = false,
                 int  PWM_state  = 0);
    ~driver_motor();
    void write();
    void write(bool LPWM_state, bool RPWM_state,int PWM_state);
    void setLPWMstate(bool state);
    void setRPWMstate(bool state);
    void setPWMstate(int state);
    bool getLPWMstate();
    bool getRPWMstate();
    int getPWMstate();
    operator int() const;
private:
    int  pin_LPWM_;             // пин LPWM
    int  pin_RPWM_;             // пин RPWM
    int  pin_PWM_;              // пин PWM
    bool LPWM_state_;           // состояние LPWM
    bool RPWM_state_;           // состояние RPWM
    int  PWM_state_;            // состояние PWM
};


driver_motor::driver_motor(int pin_LPWM, int pin_RPWM, int pin_PWM, bool LPWM_state, bool RPWM_state, int PWM_state)
{
    pin_LPWM_   = pin_LPWM;
    pin_RPWM_   = pin_RPWM;
    pin_PWM_    = pin_PWM;
    LPWM_state_ = LPWM_state;
    RPWM_state_ = RPWM_state;
    PWM_state_  = PWM_state;

    /// pinMode(pin_LPWM_, OUTPUT);
    /// pinMode(pin_RPWM_, OUTPUT);
    //  pinMode(pin_PWM_, OUTPUT);

    write();
}

driver_motor::~driver_motor()
{
    pin_LPWM_ = 0;
    pin_RPWM_ = 0;
    pin_PWM_  = 0;
    LPWM_state_ = false;
    RPWM_state_ = false;
    PWM_state_  = 0;
}

void driver_motor::write()
{
    /// digitalWrite(pin_LPWM_, LPWM_state_? HIGH : LOW);
    /// digitalWrite(pin_RPWM_, RPWM_state_? HIGH : LOW);
    /// analogWrite(pin_PWM_, PWM_state_);
}

void driver_motor::write(bool LPWM_state, bool RPWM_state, int PWM_state)
{
    LPWM_state_ = LPWM_state;
    RPWM_state_ = RPWM_state;
    PWM_state_  = PWM_state;

    write();
}

void driver_motor::setLPWMstate(bool state)
{
    LPWM_state_ = state;
}

void driver_motor::setRPWMstate(bool state)
{
    RPWM_state_ = state;
}

void driver_motor::setPWMstate(int state)
{
    PWM_state_ = state;
}

bool driver_motor::getLPWMstate()
{
    return LPWM_state_;
}

bool driver_motor::getRPWMstate()
{
    return RPWM_state_;
}

int driver_motor::getPWMstate()
{
    return PWM_state_;
}

driver_motor::operator int() const
{
    return PWM_state_;
}


#endif // DRIVER_MOTOR_H
