#ifndef DRIVER_MOTOR_H
#define DRIVER_MOTOR_H

#include <Arduino.h>
//================================================================
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
//================================================================

class driver_motor{
public:
    driver_motor(int pin_LPWM = 2,
                 int pin_RPWM = 4,
                 int pin_PWM  = 3,
                 bool LPWM_state = false,
                 bool RPWM_state = false,
                 int  PWM_state  = 0,
                 int  frequency = 800);
    ~driver_motor();
    void write();
    void write(bool LPWM_state, bool RPWM_state,int PWM_state);
    void setLPWMstate(bool state);
    void setRPWMstate(bool state);
    void setPWMstate(int state);
    bool getLPWMstate();
    bool getRPWMstate();
    int  getPWMstate();
    void drive(int pwm = 0);
    void reverse(int pwm = 0);
    void neutral();
    operator int() const;
private:
    int  pin_LPWM_;             // пин LPWM
    int  pin_RPWM_;             // пин RPWM
    int  pin_PWM_;              // пин PWM
    bool LPWM_state_;           // состояние LPWM
    bool RPWM_state_;           // состояние RPWM
    int  PWM_state_;            // состояние PWM
};


driver_motor::driver_motor(int pin_LPWM, int pin_RPWM, int pin_PWM, bool LPWM_state, bool RPWM_state, int PWM_state, int  frequency)
{
    pin_LPWM_   = pin_LPWM;
    pin_RPWM_   = pin_RPWM;
    pin_PWM_    = pin_PWM;
    LPWM_state_ = LPWM_state;
    RPWM_state_ = RPWM_state;
    PWM_state_  = PWM_state;

    pinMode(pin_LPWM_, OUTPUT);
    pinMode(pin_RPWM_, OUTPUT);
    pinMode(pin_PWM_, OUTPUT);
    /*
         Table 17-8. Waveform Generation Mode Bit Description
       Mode  | WGM2   |  WGM1  | WGM0  | Timer/Counter Mode | TOP   | Update of |  TOVn Flag
             |        |        |       | of Operation       |       |  OCRnx at |   Set on
       ------------------------------------------------------------------------------------------------
         5       1         0       1     PWM, Phase Correct   OCRA       TOP        BOTTOM
     */
    TCCR0A = ((0 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0) |
               (0 << WGM01)  | (1 << WGM00));
    /*
     * CS02   CS01  CS00  Description
     *  0     0     0     No clock source (Timer/Counter stopped)
     *  0     0     1     clkI/O / 1 (No prescaling)
     *  0     1     0     clkI/O / 8 (From prescaler)
     *  0     1     1     clkI/O / 64 (From prescaler)
     *  1     0     0     clkI/O / 256 (From prescaler)
     *  1     0     1     clkI/O / 1024 (From prescaler)
     */
    TCCR0B = ((0 << FOC0A) | (0 << FOC0B) | (1 << WGM02) | (0 << CS02) | (1 << CS01) | (0 << CS00));
    /* for Prescaler = clkI/O / 8
     * OCR0A = 10  - 100 kHz
     * OCR0A = 20  - 50 kHz
     * OCR0A = 25  - 40 kHz
     * OCR0A = 30  - 33.33 kHz
     * OCR0A = 40  - 25 kHz
     * OCR0A = 50  - 20 kHz
     * OCR0A = 64  - 15.625 kHz
     * OCR0A = 80  - 12.5 kHz
     * OCR0A = 100 - 10 kHz
     * OCR0A = 125 - 8 kHz
     * OCR0A = 160 - 6.25 kHz
     * OCR0A = 200 - 5 kHz
     * OCR0A = 250 - 4 kHz
     */
    OCR0A = frequency;
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
    write();
}

void driver_motor::write()
{
    digitalWrite(pin_LPWM_, LPWM_state_? HIGH : LOW);
    digitalWrite(pin_RPWM_, RPWM_state_? HIGH : LOW);

    static int fill_factor = map(PWM_state_, 0, 1023, 0, OCR0A);

    // Добавим мёртвую зону для полного открытия/закрытия MOSFET-а
    if (fill_factor <= 2)
    {
        digitalWrite(pin_PWM_, LOW);
    }
    else if (fill_factor >= (OCR0A-2))
    {
        digitalWrite(pin_PWM_, HIGH);
    }
    else
    {
        sbi(TCCR0A, COM0B1);
        OCR0B = fill_factor; // set pwm duty
    }
    //analogWrite(pin_PWM_, PWM_state_);
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

void driver_motor::drive(int pwm)
{
    LPWM_state_ = false;
    RPWM_state_ = true;
    PWM_state_  = pwm;
    write();
}

void driver_motor::reverse(int pwm)
{
    LPWM_state_ = true;
    RPWM_state_ = false;
    PWM_state_  = pwm;
    write();
}

void driver_motor::neutral()
{
    LPWM_state_ = false;
    RPWM_state_ = false;
    PWM_state_  = 0;
    write();
}

driver_motor::operator int() const
{
    return PWM_state_;
}


#endif // DRIVER_MOTOR_H
