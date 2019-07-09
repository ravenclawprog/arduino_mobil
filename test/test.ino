#include <Arduino.h>
#include "led_array.h"
#include "music_box.h"
#include "zeppelin_pwm.h"

int buzzer_pin = 6;                                         // пин бузера
int led_pin[] = {3, 7, 8, 9, 10, 11, 12, 13, A0};           // пины светодиодов
int slider_pin = A7;                                        // Пин подстроечного резистора
// мелодия
int melody[]    = {NOTE_G4, NOTE_C5, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_C4};
// длительность
int duration[]  = {8, 4, 11, 16, 4, 8, 8, 4, 11, 16, 4, 4};

LED_array stripLight(&led_pin[0],sizeof(led_pin)/sizeof(int));
music_box tuner(buzzer_pin, &melody[0],&duration[0],sizeof(melody)/sizeof(int),2000);
ZEPPELIN_PWM slider(slider_pin);
int range_of_slider = 1015;

bool stop_led = false;
bool stop_tune= false;
void setup() {
  // put your setup code here, to run once:
}

void loop() {
    /// Запуск работы светодиодной ленты
    if(!stop_led){
        stripLight.operate(LED_array::SALUTE_STATE);
    }
    /// Запуск работы пьезоэлектрика
    if(!stop_tune){
        tuner.play();
        tuner.operate();
    }

    /// Остановка приветственного огня
    if(stripLight.get_state() == LED_array::IDLE_STATE){
        stop_led = true;
    }

    /// Остановка воспроизведения музыки
    if(!tuner.isPlay()){
        stop_tune = true;
        tuner.stop();
        tuner.operate();
    }

    /// Функционирование в стандартном режиме - проверка работы ползунка и ленты светодиодов
    if(stop_tune && stop_led){
        slider.read();
        stripLight.operate(LED_array::DISPLAY_STATE,slider.getStatus(), range_of_slider);
    }

}
