#ifndef PROGRAM_SETTINGS_H
#define PROGRAM_SETTINGS_H

#include <Arduino.h>
#include "pitches.h"
/*=======================================*/

/*Настройки динамика*/
const int buzzer_pin = 6;                                       // пин динамика
const unsigned long TEMP_OF_MUSIC = 2000;                       // определяет длительность целой ноты в миллисекундах
// мелодия
int melody[]    = {NOTE_G4, NOTE_C5, NOTE_G4,
                   NOTE_A4, NOTE_B4, NOTE_E4,
                   NOTE_E4, NOTE_A4, NOTE_G4,
                   NOTE_F4, NOTE_G4, NOTE_C4};                  // значение ноты, описывается в файле pitches.h
// длительность нот в мелодии
int duration[]  = {8, 4, 11, 16, 4, 8, 8, 4, 11, 16, 4, 4};     // длительность  - знаменатель, на которую делится целая часть (целая - 1, половинная - 2, четверть - 4, восьмая - 8 и т.д.)
/*=======================================*/

/*Настройки ленты светодиодов*/
int led_pin[]  = { 3, 7, 8, 9, 10, 11, 12, 13, 14, 15 };        // пины светодиодов
const unsigned long FREQUENCY_OF_BLINK = 500;                   // частота мерция лампочек в режиме "Салют" массива светодиодов
/*=======================================*/

/*Настройка реостата*/
const int slider_pin = A7;                                      // пин реостата
/*=======================================*/

/*Настройки двигателя*/
const int lpwm_pin   = 2;                                         // пин входа lpwm драйвера
const int rpwm_pin   = 5;                                         // пин входа rpwm драйвера
const int pwm_pin    = 4;                                         // пин входа pwm драйвера

const double max_speed = 255.0;
const double min_speed = 0.0;
const double increase_speed = 0.5;
const double decrease_speed = 0.5;
const double start_speed = 90;
const double stop_speed = 0;
const unsigned long time_to_change = 100;

/*=======================================*/

/*Настройка педалей*/
unsigned long ANTIDEBOUNCE_TIME = 200;                          //
const int pin_accelerate = A1; //педаль газа (в нажатом положении LOW)
const int pin_drive = A3; //положение рычага КПП (в положении ЕХАТЬ ВПЕРЕД LOW)
const int pin_revers = A4; //положение рычага КПП (в положении ЕХАТЬ НАЗАД LOW)
#endif // PROGRAM_SETTINGS_H
