#ifndef PROGRAM_SETTINGS_H
#define PROGRAM_SETTINGS_H

#ifdef QT_ARDUINO_TEST
#include "Arduino.h"
#else
#include <Arduino.h>
#endif

#include "pitches.h"
/*=======================================*/
#ifdef QT_ARDUINO_TEST
/*Настройки динамика*/
const int buzzer_pin = A0;                                       // пин динамика
const unsigned long TEMP_OF_MUSIC = 4000;                       // определяет длительность целой ноты в миллисекундах
// мелодия
int melody[]    = {NOTE_G4, NOTE_C5, NOTE_G4,
                   NOTE_A4, NOTE_B4, NOTE_E4,
                   NOTE_E4, NOTE_A4, NOTE_G4,
                   NOTE_F4, NOTE_G4, NOTE_C4};                  // значение ноты, описывается в файле pitches.h
// длительность нот в мелодии
int duration[]  = {8, 4, 11, 16, 4, 8, 8, 4, 11, 16, 4, 4};     // длительность  - знаменатель, на которую делится целая часть (целая - 1, половинная - 2, четверть - 4, восьмая - 8 и т.д.)

int reverse_melody[] = {NOTE_A7, 0};
int reverse_melody_duration[] = {4,4};
/*=======================================*/

/*Настройки ленты светодиодов*/
int led_pin[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };        // пины светодиодов
const unsigned long FREQUENCY_OF_BLINK = 500;                   // частота мерция лампочек в режиме "Салют" массива светодиодов
/*=======================================*/

/*Настройка реостата*/
const int slider_pin = 11;                                      // пин реостата
/*=======================================*/

/*Настройки двигателя*/
const int lpwm_pin   = A0;                                         // пин входа lpwm драйвера
const int rpwm_pin   = A1;                                         // пин входа rpwm драйвера
const int pwm_pin    = 10;                                         // пин входа pwm драйвера

const double max_speed = 255.0;                                    // минимальная скорость - выход PWM
const double min_speed = 0.0;                                      // максимальная скорость - выход PWM
const double increase_speed = 0.15;                                // значение ускорения
const double decrease_speed = 0.15;                                // значение торможения
const double start_speed = 90;                                     // начальная скорость
const double stop_speed = 0;                                       // конечная скорость
const unsigned long time_to_change = 100;                          // такт работы драйвера - не используется

/*=======================================*/

/*Настройка педалей*/
unsigned long ANTIDEBOUNCE_TIME = 200;                          //
const int pin_accelerate = A3; //педаль газа (в нажатом положении LOW)
const int pin_drive = A4; //положение рычага КПП (в положении ЕХАТЬ ВПЕРЕД LOW)
const int pin_revers = A5; //положение рычага КПП (в положении ЕХАТЬ НАЗАД LOW)
#else
/*Настройки динамика*/
const int buzzer_pin = 6;                                       // пин динамика
const unsigned long TEMP_OF_MUSIC = 900;                       // определяет длительность целой ноты в миллисекундах
// мелодия
int melody[]    = {NOTE_C5, NOTE_G4,NOTE_E4,
                   NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
                   NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5,NOTE_F5,NOTE_G5,
                   0,NOTE_E5, NOTE_C5, NOTE_D5, NOTE_B4,0};                  // значение ноты, описывается в файле pitches.h
// длительность нот в мелодии
int duration[]  = {4, 4, 4,
                   4, 4, 8, 4,
                   12,12,12,4,8,8,
                   16,4,8,8,4,16};     // длительность  - знаменатель, на которую делится целая часть (целая - 1, половинная - 2, четверть - 4, восьмая - 8 и т.д.)

int reverse_melody[] = {NOTE_A6, 0};
int reverse_melody_duration[] = {2,2};
/*=======================================*/

/*Настройки ленты светодиодов*/
int led_pin[]  = { 3, 7, 8, 9, 10, 11, 12, 13, 14};        // пины светодиодов
const unsigned long FREQUENCY_OF_BLINK = 100;                   // частота мерция лампочек в режиме "Салют" массива светодиодов
/*=======================================*/

/*Настройка реостата*/
const int slider_pin = A7;                                      // пин реостата
/*=======================================*/

/*Настройки двигателя*/
const int lpwm_pin   = 2;                                         // пин входа lpwm драйвера
const int rpwm_pin   = 4;                                         // пин входа rpwm драйвера
const int pwm_pin    = 5;                                         // пин входа pwm драйвера

const double max_speed = 255.0;                                    // минимальная скорость - выход PWM
const double min_speed = 0.0;                                      // максимальная скорость - выход PWM
const double increase_speed = 0.25;                                // значение ускорения
const double decrease_speed = 0.25;                                // значение торможения
const double start_speed = 90;                                     // начальная скорость
const double stop_speed = 0;                                       // конечная скорость
const unsigned long time_to_change = 100;                          // такт работы драйвера - не используется

/*=======================================*/

/*Настройка педалей*/
unsigned long ANTIDEBOUNCE_TIME = 200;                          //
const int pin_accelerate = A1; //педаль газа (в нажатом положении LOW)
const int pin_drive = A3; //положение рычага КПП (в положении ЕХАТЬ ВПЕРЕД LOW)
const int pin_revers = A4; //положение рычага КПП (в положении ЕХАТЬ НАЗАД LOW)
#endif
#endif // PROGRAM_SETTINGS_H

