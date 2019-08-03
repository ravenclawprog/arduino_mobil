#ifndef MUSIC_BOX_H
#define MUSIC_BOX_H

#include "pitches.h"
#include <Arduino.h>


///
/// \brief The music_box class
/// класс позволяет воспроизводить музыку, при помощи динамика
///

class music_box {
public:
    enum music_box_state {                      // состояние музыкальной шкатулки
        UNDEFINED_STATE = 0,                    // неопределенное состояние
        STOP_STATE,				// состояние стоп
        PAUSE_STATE,                            // состояние пауза
        PLAY_STATE                              // состояние проигрывание
    };
    /// конструктор по умолчанию - задает начальные параметры переменных
    music_box();
    /// благодаря template мы можем не дублировать данные в динамической памяти
    /// более того, теперь при несовпадении размеров массивов, мы будем осведомлены на этапе компиляции
    /// music_box - конструктор, устанавливает мелодию с её продолжительностью, пин, параметр повторения музыки, темп воспроизведения
    
    template <size_t n_> music_box(int pin, int (&melody)[n_], int (&duration)[n_], bool repeat = false, unsigned long temp = 1200);
    /// setNewSong - метод останавливает текущее воспроизведение музыки, устанавливает другую мелодию
    
    template <size_t n_> void setNewSong(int (&melody)[n_], int (&duration)[n_], bool repeat = false, unsigned long temp = 1200);
    /// getState - метод возвращает текущее состояние объекта
    music_box_state getState();
    /// getPosition - показывает, в какой текущей позиции воспроизводится мелодия
    int getPosition();
    /// getLength - метод возвращает длину массивов melody и duration
    int getLength();
    /// getTemp - метод возвращает текущий темп воспроизведения
    unsigned long getTemp();
    /// setRepeat - метод устанавливает флаг повторения музыки.
    /// Прим.: данный флаг работает точно также, как и в музыкальных плеерах
    /// т.е. если установить данный флаг не в режиме воспроизведения, то повторения музыки не произойдет (точнее, воспроизведения с последующим повторением)
    void setRepeat();
    /// resetRepeat - метод убирает флаг повторения музыки.
    void resetRepeat();
    /// pause - метод останавливает вопроизведение музыки без сброса текущей позиции вопроизведения.
    void pause();
    /// play - метод подает заданную частоту (в указанной позиции массива melody) и удерживает ее в течении времени (указанного в позиции массива duration) выдавая ее на пин, указанный при создании объекта
    void play();
    /// stop - метод останавливает воспроизведение музыки.
    void stop();
    /// isPlay - метод возвращает true, если сейчас происходит вопроизведение музыки и false - в противном случае
    bool isPlay();
    /// isValid - метод возвращает true, если массивы melody и duration были переданы в объект
    bool isValid();
    /// деструктор - останавливает воспроизведение музыки.
    ~music_box();
private:
    int  pin_;                  // пин, к которому подключен динамик
    int* melody_;               // мелодия
    int* duration_;             // длительность
    size_t  a_length_;          // размер массивов

    bool repeat_;               // будет ли мелодия повторяться
    unsigned long  temp_;       // темп воспроизводимой музыки
    music_box_state mb_state_;  // состояние объекта класса

    /// Переменные для проигрывания музыки

    int position_;              // текущая проигрываемая позиция в массиве melody_
    bool end_playing_;          // флаг - признак окончания воспроизведения
    unsigned long last_time_;   // переменная времени

    /// Дополнительыне функции
    void init();                // инициализация
};



template<size_t n_> music_box::music_box(int pin, int (&melody)[n_], int (&duration)[n_], bool repeat, unsigned long temp)
{
    mb_state_ = music_box::STOP_STATE;
    a_length_ = sizeof(melody)/sizeof(melody[0]);
    melody_   = melody;
    duration_ = duration;
    temp_     = temp;
    pin_      = pin;
    repeat_   = repeat;
    end_playing_ = false;
    position_    = 0;
    pinMode(pin_, OUTPUT);
}


template<size_t n_> void music_box::setNewSong(int (&melody)[n_], int (&duration)[n_], bool repeat, unsigned long temp)
{
    stop();
    mb_state_ = music_box::STOP_STATE;
    a_length_ = sizeof(melody)/sizeof(melody[0]);
    melody_   = melody;
    duration_ = duration;
    temp_     = temp;
    repeat_   = repeat;
}

music_box::music_box()
{
    init();
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
    if(mb_state_ != music_box::PLAY_STATE){
        end_playing_ = false;
        last_time_ = millis();
    }
    mb_state_ = music_box::PLAY_STATE;
    tone(pin_,melody_[position_]);
    if(millis() - last_time_ > (unsigned long)(temp_/duration_[position_])){
        position_++;
        noTone(pin_);
        last_time_ = millis();
    }
    if(position_ >= a_length_){
        last_time_ = millis();
        if(repeat_) {
            position_ = 0;
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

bool music_box::isValid()
{
    return (melody_ != NULL) && (duration_ != NULL);
}

music_box::~music_box()
{
    stop();
    init();
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
