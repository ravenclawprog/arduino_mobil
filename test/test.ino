#include "led_array.h"
#include "music_box.h"
#include "zeppelin_pwm.h"
#include "driver_motor.h"
#include "ZEPPELIN.h"
#include "program_settings.h"


const int range_of_slider = 1015;


LED_array<sizeof(led_pin)/sizeof(led_pin[0])>    stripLight(led_pin,
                        false,
                        FREQUENCY_OF_BLINK); //template-constructor нельзя так просто специализировать
music_box<sizeof(melody)/sizeof(melody[0])>    tuner(buzzer_pin,
                   melody,
                   duration,
                   false,
                   TEMP_OF_MUSIC);
ZEPPELIN_PWM slider(slider_pin);
driver_motor driver(lpwm_pin,
                    rpwm_pin,
                    pwm_pin,
                    start_speed,
                    stop_speed,
                    increase_speed,
                    decrease_speed,
                    time_to_change,
                    max_speed,
                    min_speed);
ZEPPELIN accelerate(false,
                    pin_accelerate,
                    true,
                    ANTIDEBOUNCE_TIME);
ZEPPELIN drive(false,
                    pin_drive,
                    true,
                    ANTIDEBOUNCE_TIME);
ZEPPELIN reverse(false,
                    pin_revers,
                    true,
                    ANTIDEBOUNCE_TIME);
bool stop_led = false;
bool stop_tune= false;

void setup() {

}

void loop() {
    if(stop_led == false || stop_tune == false){
        /// Запуск работы светодиодной ленты
        if(!stop_led){
            stripLight.salute();
        }
        /// Запуск работы пьезоэлектрика
        if(!stop_tune){
            tuner.play();
        }

        /// Остановка приветственного огня
        if(stripLight.is_salute_end()){
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
        stripLight.display_number(slider.get_state(),range_of_slider);
        accelerate.read();
        drive.read();
        reverse.read();
        if(accelerate){
            if(drive){
                driver.drive(map(slider.get_state(), 0, range_of_slider,
                                 static_cast<int>(driver.get_start_speed()),
                                 static_cast<int>(driver.get_max_speed())));
            } else if(reverse) {
                driver.reverse(map(slider.get_state(), 0, range_of_slider, static_cast<int>(driver.get_start_speed()),
                                   static_cast<int>(driver.get_max_speed())));
            } else {
                driver.neutral();
            }
        } else {
            if(drive){
                driver.drive();
            } else if(reverse){
                driver.reverse();
            } else {
                driver.neutral();
                driver.set_new_speed(driver.get_stop_speed());
            }

        }

    }

}

