#ifndef DRIVER_MOTOR_H
#define DRIVER_MOTOR_H

#ifdef QT_ARDUINO_TEST
#include "Arduino.h"
#else
#include <Arduino.h>
#endif

class driver_motor{
public:
    driver_motor(int pin_LPWM = 2,
                 int pin_RPWM = 4,
                 int pin_PWM  = 3,
                 double start_speed = 90,
                 double stop_speed  = 0,
                 double increase_speed = 0.5,
                 double decrease_speed = 0.5,
                 unsigned long time_to_change = 100,
                 double max_speed = 255,
                 double min_speed = 0,
                 bool LPWM_state = false,
                 bool RPWM_state = false,
                 int  PWM_state  = 0);
    ~driver_motor();
    void drive(double speed = 0);
    void reverse(double speed = 0);
    void not_accelerate_drive();
    void not_accelerate_reverse();
    void neutral();
    double get_start_speed();
    double get_stop_speed();
    double get_max_speed();
    double get_min_speed();
    int    get_PWM_state();
    void set_new_speed(double new_speed);   // устанавливает скорость с учетом ускорения и торможнения
    operator int() const;
private:
    int  pin_LPWM_;             // пин LPWM
    int  pin_RPWM_;             // пин RPWM
    int  pin_PWM_;              // пин PWM
    bool LPWM_state_;           // состояние LPWM
    bool RPWM_state_;           // состояние RPWM
    int  PWM_state_;            // состояние PWM
    double speed_;              // требуемая скорость
    double increase_speed_;     // ускорение при разгоне
    double decrease_speed_;     // ускорение при торможении
    double start_speed_;        // начальная скорость
    double stop_speed_;         // скорость остановки
    double max_speed_;          // максимальная скорость
    double min_speed_;          // минимальная скорость
    unsigned long time_to_change_;// время, через которое будет изменяться скорость
    bool drive_first_loop_;
    ///////

    void write();
    void write(bool LPWM_state, bool RPWM_state,int PWM_state);

    void check_speed();                     // функция проверки скорости

    void inc_speed();                       // функция увеличения скорости
    void dec_speed();                       // функция уменьшения скорости
};

driver_motor::driver_motor(int pin_LPWM, int pin_RPWM, int pin_PWM,
                           double start_speed, double stop_speed,
                           double increase_speed, double decrease_speed,
                           unsigned long time_to_change,
                           double max_speed,double min_speed,
                           bool LPWM_state, bool RPWM_state, int PWM_state)
{
    pin_LPWM_   = pin_LPWM;
    pin_RPWM_   = pin_RPWM;
    pin_PWM_    = pin_PWM;
    LPWM_state_ = LPWM_state;
    RPWM_state_ = RPWM_state;
    PWM_state_  = PWM_state;
    start_speed_ = start_speed;
    stop_speed_  = stop_speed;
    increase_speed_ = increase_speed;
    decrease_speed_ = decrease_speed;
    time_to_change_ = time_to_change;   // пока что не используется, но при помощи millis и inc и dec speed можно устроить тактирование по времени
    max_speed_      = max_speed;
    min_speed_      = min_speed;
    drive_first_loop_ = true;

    pinMode(pin_LPWM_, OUTPUT);
    pinMode(pin_RPWM_, OUTPUT);
    pinMode(pin_PWM_, OUTPUT);
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

    analogWrite(pin_PWM_, PWM_state_);
}

void driver_motor::write(bool LPWM_state, bool RPWM_state, int PWM_state)
{
    LPWM_state_ = LPWM_state;
    RPWM_state_ = RPWM_state;
    PWM_state_  = PWM_state;

    write();
}

void driver_motor::check_speed()
{
    if(speed_ > max_speed_) speed_ = max_speed_;
    if(speed_ < min_speed_) speed_ = min_speed_;
}

void driver_motor::set_new_speed(double new_speed)
{
    double sub =  speed_ - new_speed;
    if(sub > 0){
        dec_speed();
    }else if(sub < 0){
        inc_speed();
    } else {
        speed_ = new_speed;
    }
    if((sub > 0 && sub <= decrease_speed_) ||
           (sub < 0 && -sub <= increase_speed_)) {
          speed_ = new_speed;
    }
    check_speed();
}

void driver_motor::inc_speed()
{
    speed_ += increase_speed_;
}

void driver_motor::dec_speed()
{
    speed_ -= decrease_speed_;
}

void driver_motor::drive(double speed)
{
    LPWM_state_ = false;
    RPWM_state_ = true;
    if(drive_first_loop_){
        speed_ = start_speed_;
        drive_first_loop_ = false;
    } else {
        set_new_speed(speed);
    }
    PWM_state_  = static_cast<int>(speed_);
    write();
}

void driver_motor::reverse(double speed)
{
    LPWM_state_ = true;
    RPWM_state_ = false;
    if(drive_first_loop_){
        speed_ = start_speed_;
        drive_first_loop_ = false;
    } else {
        set_new_speed(speed);
    }
    //set_new_speed(speed);
    PWM_state_  = static_cast<int>(speed_);
    write();
}

void driver_motor::not_accelerate_drive()
{
    LPWM_state_ = false;
    RPWM_state_ = true;
    set_new_speed(min_speed_);
    PWM_state_  = 0;
    write();
}

void driver_motor::not_accelerate_reverse()
{
    LPWM_state_ = true;
    RPWM_state_ = false;
    set_new_speed(min_speed_);
    PWM_state_  = 0;
    write();
}

void driver_motor::neutral()
{
    LPWM_state_ = false;
    RPWM_state_ = false;
    //speed_ = stop_speed_;
    PWM_state_  = 0;//static_cast<int>(speed_);
    write();
}

double driver_motor::get_start_speed()
{
    return  start_speed_;
}

double driver_motor::get_stop_speed()
{
    return  stop_speed_;
}

double driver_motor::get_max_speed()
{
    return  max_speed_;
}

double driver_motor::get_min_speed()
{
    return  min_speed_;
}

int driver_motor::get_PWM_state()
{
    return PWM_state_;
}

driver_motor::operator int() const
{
    return speed_;
}


#endif // DRIVER_MOTOR_H
