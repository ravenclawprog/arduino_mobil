#include <Arduino.h>
#include "led_array.h"
#include "music_box.h"
#include "zeppelin_pwm.h"
#include "driver_motor.h"

const int buzzer_pin = 6;                                         // пин динамика
      int led_pin[]  = { 3, 7, 8, 9, 10, 11, 12, 13, 14, 15 };    // пины светодиодов
const int slider_pin = A7;                                        // пин подстроечного резистора
const int lpwm_pin   = 2;                                         // пин входа lpwm драйвера
const int rpwm_pin   = 5;                                         // пин входа rpwm драйвера
const int pwm_pin    = 4;                                         // пин входа pwm драйвера
const int range_of_slider = 1015;
// мелодия
int melody[]    = {NOTE_G4, NOTE_C5, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_C4};
// длительность
int duration[]  = {8, 4, 11, 16, 4, 8, 8, 4, 11, 16, 4, 4};

LED_array    stripLight(&led_pin[0],
                        sizeof(led_pin)/sizeof(led_pin[0]) );
music_box    tuner(buzzer_pin,
                   &melody[0],
                   &duration[0],
                   sizeof(melody)/sizeof(melody[0]),
                   2000);
ZEPPELIN_PWM slider(slider_pin);
driver_motor driver(lpwm_pin,
                    rpwm_pin,
                    pwm_pin);


bool stop_led = false;
bool stop_tune= false;
void setup() {

}

void loop() {
    if(stop_led != true && stop_tune != true){
        /// Запуск работы светодиодной ленты
        if(!stop_led){
            stripLight.salute();
        }
        /// Запуск работы пьезоэлектрика
        if(!stop_tune){
            tuner.play();
        }

        /// Остановка приветственного огня
        if(stripLight.isSaluteEnd()){
            stripLight.idle();
            stop_led = true;
        }

        /// Остановка воспроизведения музыки
        if(!tuner.isPlay()){
            tuner.stop();
            stop_tune = true;
        }
    } else if(stop_led == true && stop_tune == true){

        /// Функционирование в стандартном режиме - проверка работы ползунка, ленты светодиодов и движения мотора
        slider.read();
        stripLight.display_number(slider.getState(),range_of_slider);
        driver.drive(map(slider.getState(), 0, 1023, 60, 255));
    }

}
