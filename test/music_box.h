#ifndef MUSIC_BOX_H
#define MUSIC_BOX_H

#include "pitches.h"
#include <Arduino.h>


class music_box{    // музыкальная шкатулка не обрабатывает ошибки, когда duration и melody имеют разный размер и когда заднный размер не совпадает с a_length_. Поэтому будьте осторожны при использовании!!! - данную ошибку тяжело отследить из-за динамической памяти! Возможные пути решения - передача в качестве аргументов массивов фиксированной длины (да, неудобно, но зато избавит от последующих проблем).
public:
    enum music_box_state {
        UNDEFINED_STATE = 0,
        STOP_STATE,
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
    bool pause_between_notes_;
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
             pause_between_notes_ = false;
        } break;
        case music_box::PLAY_STATE:{
            if(!pause_between_notes_){
                tone(pin_,melody_[position_]);
                if(millis() - last_time_ > (unsigned long)((double)duration_[position_]*(double)(1/temp_))){
                    position_++;
                    last_time_ = millis();
                    pause_between_notes_ = true;
                }
            } else {
                if(millis() - last_time_ > (unsigned long)(0.05f*temp_)){
                    noTone(pin_);
                    last_time_ = millis();
                    pause_between_notes_ = false;
                }
            }
            if(position_ >= a_length_){
                mb_state_ = music_box::STOP_STATE;
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

#endif // MUSIC_BOX_H
