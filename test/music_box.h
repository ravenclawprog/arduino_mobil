#ifndef MUSIC_BOX_H
#define MUSIC_BOX_H

#include "pitches.h"
#include <Arduino.h>


///
/// \brief The music_box class
/// класс позволяет воспроизводить музыку, передаваемую в массив, через бузер
///

class music_box{    // музыкальная шкатулка не обрабатывает ошибки, когда duration и melody имеют разный размер и когда заднный размер не совпадает с a_length_. Поэтому будьте осторожны при использовании!!! - данную ошибку тяжело отследить из-за динамической памяти! Возможные пути решения - передача в качестве аргументов массивов фиксированной длины (да, неудобно, но зато избавит от последующих проблем).
public:
    enum music_box_state {                      // состояние музыкальной шкатулки
        UNDEFINED_STATE = 0,                    // неопределенное состояние
        STOP_STATE,				// состояние стоп
        PAUSE_STATE,                            // состояние пауза
        PLAY_STATE                              // состояние проигрывание
    };
    music_box();
    music_box(int pin, int *melody, int *duration, int a_length,bool repeat = false, unsigned long temp = 1200);
    inline void copyMelodyToMusicalBox(int *melody, int *duration, int a_length, bool repeat = false, unsigned long temp = 1200);
    inline void operate();
    inline music_box_state getState();
    inline int getPosition();
    inline int getLength();
    inline unsigned long getTemp();
    inline void setRepeat();
    inline void resetRepeat();
    inline void pause();
    inline void play();
    inline void stop();
    inline bool isPlay();
    ~music_box();
private:
    int  pin_;                  // пин, к которому подключен динамик
    int* melody_;               // мелодия
    int* duration_;             // длительность
    int  a_length_;             // размер массивов
    bool repeat_;               // будет ли мелодия повторяться
    unsigned long  temp_;
    music_box_state mb_state_;

    /// Переменные для проигрывания музыки

    int position_;              // текущая проигрываемая позиция в массиве melody_
    bool end_playing_;          // флаг - признак окончания воспроизведения
    unsigned long last_time_;   // переменная времени

    /// Дополнительыне функции
    inline void erase();        // очищает массив melody И duration, переводя музыкальную коробку в состояние СТОП
    inline void init();         // инициализация
};



music_box::music_box()
{
    init();
}

music_box::music_box(int pin, int *melody, int *duration, int a_length, bool repeat, unsigned long temp)
{
    if(a_length > 0) {
        mb_state_ = music_box::STOP_STATE;
        a_length_ = a_length;
        melody_   = new int[a_length_];
        duration_ = new int[a_length_];
        temp_     = temp;
        pin_      = pin;
        repeat_   = repeat;
        for(int i = 0; i < a_length_; i++){
           melody_[i]   = melody[i];
           duration_[i] = duration[i];
        }
        pinMode(pin_, OUTPUT);
    } else {
        init();
    }
}

void music_box::copyMelodyToMusicalBox(int *melody, int *duration, int a_length, bool repeat, unsigned long temp)
{
    erase();
    if(a_length > 0) {
        mb_state_ = music_box::STOP_STATE;
        a_length_ = a_length;
        melody_   = new int[a_length_];
        duration_ = new int[a_length_];
        temp_     = temp;
        repeat_   = repeat;
        for(int i = 0; i < a_length_; i++){
           melody_[i]   = melody[i];
           duration_[i] = duration[i];
        }
    } else {
        init();
    }
}

void music_box::operate()
{
    switch (mb_state_) {
        case music_box::UNDEFINED_STATE:{
            noTone(pin_);
        } break;
        case music_box::PAUSE_STATE:{
            pause();
        } break;
        case music_box::STOP_STATE:{
            stop();
        } break;
        case music_box::PLAY_STATE:{
            play();
        } break;
    }
}

music_box::music_box_state music_box::getState()
{
    return mb_state_;
}

int music_box::getPosition()
{
    return position_;
}

int music_box::getLength()
{
    return a_length_;
}

unsigned long music_box::getTemp()
{
    return temp_;
}

void music_box::setRepeat()
{
    repeat_ = true;
}

void music_box::resetRepeat()
{
    repeat_ = false;
}

void music_box::pause()
{
    mb_state_ = music_box::PAUSE_STATE;
    noTone(pin_);
    last_time_ = millis();
}

void music_box::play()
{
    mb_state_ = music_box::PLAY_STATE;
    tone(pin_,melody_[position_]);
    if((!end_playing_)&&(millis() - last_time_ > (unsigned long)(temp_/duration_[position_]))){
        position_++;
        noTone(pin_);
        last_time_ = millis();
    }
    if(position_ >= a_length_){
        if(repeat_){
            position_ = 0;
            last_time_ = millis();
        } else {
            stop();
        }
    }
}

void music_box::stop()
{
    mb_state_ = music_box::STOP_STATE;
    noTone(pin_);
    last_time_ = millis();
    position_ = 0;
    end_playing_ = true;
}

bool music_box::isPlay()
{
    return !(end_playing_);
}

music_box::~music_box()
{
    erase();
}

void music_box::erase()
{
    mb_state_ = music_box::UNDEFINED_STATE;
    a_length_ = 0;
    if(melody_) {
            delete [] melody_;
    }
    if(duration_) {
            delete [] duration_;
    }
    melody_   = NULL;
    duration_ = NULL;
}

void music_box::init()
{
    mb_state_ = music_box::UNDEFINED_STATE;
    a_length_ = 0;
    melody_   = NULL;
    duration_ = NULL;
    temp_     = 0;
    pin_      = 6;
    repeat_   = false;
}
#endif // MUSIC_BOX_H
