#ifndef ZEPPELIN_H
#define ZEPPELIN_H
#include <Arduino.h>

class ZEPPELIN {
public:
        ZEPPELIN(bool status = false,
            int pin = 2,
            bool reverse_logic = false,
            unsigned long anti_debounce_time = 200);
        operator bool() const;                          // перегрузка оператора bool
        bool read();                                    // считать состояние цепелина

        void setReverseLogic();                         // установить режим обратной логики
        void resetReverseLogic();                       // сбросить режим обратной логики
        void setAntiDebounce(unsigned long anti_debounce_time = 200);
        void resetAntiDebounce();

        bool isChange();                                // состояние входа изменилось
        bool isClicked();                               // кнопка без фиксации состояния - была нажата
        bool isUp();                                    // состояние входа было изменено с нулевого уровня на первый
        bool isDown();                                  // состояние входа было изменено с первого на нулевой уровень
private:
        bool    out_status_;                            // выход функции
        bool    status_;                                // состояние светодиода - вкл./откл.
        int     pin_;                                   // пин, к которому подклчен светодиод
        int     click_count_;                           // количество кликов
        bool    anti_debounce_state_;                   // режим анти-дребезга
        bool    reverse_logic_;                         // обратная логика
        bool    prev_status_;                           // предыдущее значение статуса
        unsigned long anti_debounce_time_;              // время антидребезга
        unsigned long last_time_;                       // переменная времени

        bool    change_;
        bool    click_;
        bool    up_;
        bool    down_;

        void classify();                                // распознавание состояний
};

ZEPPELIN::ZEPPELIN(bool status, int pin, bool reverse_logic, unsigned long anti_debounce_time)
{
    status_ = status;
    pin_ = pin;
    reverse_logic_ = reverse_logic;
    anti_debounce_time_ = anti_debounce_time;

    last_time_ = millis();
    pinMode(pin_, INPUT);
    digitalWrite(pin_, HIGH );
    status_ = this->read();
    prev_status_ = status_;
    click_count_ = 0;
}


bool ZEPPELIN::read()
{
     status_ = reverse_logic_ ? digitalRead(pin_) == HIGH : digitalRead(pin_) == LOW;
     classify();
     prev_status_ = status_;
     return out_status_;
}

void ZEPPELIN::setReverseLogic()
{
    reverse_logic_ = true;
}

void ZEPPELIN::resetReverseLogic()
{
    reverse_logic_ = false;
}

void ZEPPELIN::setAntiDebounce(unsigned long anti_debounce_time)
{
    anti_debounce_time_ = anti_debounce_time;
}

void ZEPPELIN::resetAntiDebounce()
{
    anti_debounce_time_ = 0;
}

bool ZEPPELIN::isChange()
{
    return change_;
}

bool ZEPPELIN::isClicked()
{
    return click_;
}

bool ZEPPELIN::isUp()
{
    return up_;
}

bool ZEPPELIN::isDown()
{
    return down_;
}

void ZEPPELIN::classify()
{
    change_ = status_ ^ prev_status_;            // change

    if(change_){                                 // up and down
        up_     = (!prev_status_)&&(status_);
        down_   = (prev_status_)&&(!status_);
    }

    if(up_){
        click_ = true;
        click_count_ ++;
    }


    if(change_){
       last_time_ = millis();
       anti_debounce_state_ = true;
    }

    if((millis() - last_time_ >= anti_debounce_time_) && anti_debounce_state_){
       out_status_ = status_;
       change_ = true;              // костыльно, но всё же...
       anti_debounce_state_ = false;
    } else {
	   change_ = false;
	}

    if(!anti_debounce_state_){
        out_status_ = status_;
    }

}

ZEPPELIN::operator bool() const
{
    return out_status_;
}

#endif
