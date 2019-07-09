#ifndef MUSIC_BOX_H
#define MUSIC_BOX_H

#include "pitches.h"
#include <Arduino.h>
// ToDo: переделать класс- вместо функции operate распределить функциональ по соответствуюим методам stop,pause,lay.
// ToDo: сделать воспроизведение с зацикливанием
// ToDo: сделать возможность подавать несколько мелодий и их исполнять
class music_box{    // музыкальная шкатулка не обрабатывает ошибки, когда duration и melody имеют разный размер и когда заднный размер не совпадает с a_length_. Поэтому будьте осторожны при использовании!!! - данную ошибку тяжело отследить из-за динамической памяти! Возможные пути решения - передача в качестве аргументов массивов фиксированной длины (да, неудобно, но зато избавит от последующих проблем).
public:
    enum music_box_state {		// состояние музыкальной шкатулки
        UNDEFINED_STATE = 0,            // неопределенное состояние
        STOP_STATE,			//
        PAUSE_STATE,
        PLAY_STATE
    };
    music_box();
    music_box(int pin, int *melody, int *duration, int a_length, unsigned long temp = 1200);
    void operate();
    void pause();
    void play();
    void stop();
    bool isPlay();
    ~music_box();
private:
    int  pin_;
    int* melody_;
    int* duration_;
    int  a_length_;
    unsigned long  temp_;
    music_box_state mb_state_;

    /// Переменные для проигрывания музыки

    int position_;
    bool end_playing_;
    unsigned long last_time_;
};



music_box::music_box()
{
    mb_state_ = music_box::UNDEFINED_STATE;
    a_length_ = 0;
    melody_   = NULL;
    duration_ = NULL;
    temp_     = 0;
    pin_      = 6;
}

music_box::music_box(int pin, int *melody, int *duration, int a_length, unsigned long temp)
{
    if(a_length > 0) {
        mb_state_ = music_box::UNDEFINED_STATE;
        a_length_ = a_length;
        melody_   = new int[a_length_];
        duration_ = new int[a_length_];
        temp_     = temp;
        pin_      = pin;
        for(int i = 0; i < a_length_; i++){
           melody_[i]   = melody[i];
           duration_[i] = duration[i];
        }
        pinMode(pin_, OUTPUT);
    } else {
        mb_state_ = music_box::UNDEFINED_STATE;
        a_length_ = 0;
        melody_   = NULL;
        duration_ = NULL;
        temp_     = 0;
        pin_      = 6;
    }
}

void music_box::operate()
{
    switch (mb_state_) {
        case music_box::UNDEFINED_STATE:{
            noTone(pin_);
            mb_state_ = music_box::STOP_STATE;
        } break;
        case music_box::PAUSE_STATE:{
             noTone(pin_);
             last_time_ = millis();
        } break;
        case music_box::STOP_STATE:{
             noTone(pin_);
             last_time_ = millis();
             position_ = 0;
             end_playing_ = true;
        } break;
        case music_box::PLAY_STATE:{
            tone(pin_,melody_[position_]);
            if(millis() - last_time_ > (unsigned long)(temp_/duration_[position_])){
                position_++;
                noTone(pin_);
                last_time_ = millis();
            }
            if(position_ >= a_length_){
                mb_state_ = music_box::STOP_STATE;
                last_time_ = millis();
                position_ = 0;
                end_playing_ = true;
            }
        } break;
    }
}

void music_box::pause()
{
    mb_state_ = music_box::PAUSE_STATE;
}

void music_box::play()
{
    mb_state_ = music_box::PLAY_STATE;
}

void music_box::stop()
{
    mb_state_ = music_box::STOP_STATE;
}

bool music_box::isPlay()
{
    return !(end_playing_);
}

music_box::~music_box()
{
    mb_state_ = music_box::UNDEFINED_STATE;
    a_length_ = 0;
	if(!melody_) {
		delete [] melody_;
	}
	if(!duration_) {
		delete [] duration_;
	}
}
#endif // MUSIC_BOX_H
