#ifndef ARDUINO_MOBIL_DESKTOP_H
#define ARDUINO_MOBIL_DESKTOP_H

#include "./libs/led_array.h"
#include "./libs/music_box.h"
#include "./libs/zeppelin_pwm.h"
#include "./libs/driver_motor.h"
#include "./libs/ZEPPELIN.h"
#include "./libs/program_settings.h"


const int range_of_slider = 1015;


LED_array<sizeof(led_pin)/sizeof(led_pin[0])>    stripLight(led_pin,
                        false,
                        FREQUENCY_OF_BLINK); //template-constructor нельзя так просто специализировать
music_box<sizeof(melody)/sizeof(melody[0])>    tuner(buzzer_pin,
                   melody,
                   duration,
                   false,
                   TEMP_OF_MUSIC);
music_box<sizeof(reverse_melody)/sizeof(reverse_melody[0])>    back_tuner(buzzer_pin,
                   reverse_melody,
                   reverse_melody_duration,
                   true,
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
enum mobil_state{
    LOAD_MOBIL_STATE = 0,
    DRIVE_MOBIL_STATE,
    REVERSE_MOBIL_STATE,
    NEUTRAL_MOBIL_STATE,
    IDLE_MOBIL_STATE
};
mobil_state machine_state;
void setup() {
    machine_state = LOAD_MOBIL_STATE;
//    Serial.begin(9600);
}

void loop() {

    slider.read();
    accelerate.read();
    drive.read();
    reverse.read();
    switch(machine_state){
        case LOAD_MOBIL_STATE: {
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
            if(stop_led == true && stop_tune == true){
                machine_state = IDLE_MOBIL_STATE;
            }
        } break;
        case IDLE_MOBIL_STATE: {
            stripLight.display_number(slider.get_state(),range_of_slider);
            back_tuner.stop();
            if(drive && ! reverse){
                machine_state = DRIVE_MOBIL_STATE;
            } else if ( reverse && !drive) {
                machine_state = REVERSE_MOBIL_STATE;
            } else if (!reverse && !drive) {
                machine_state = NEUTRAL_MOBIL_STATE;
            }
        } break;
        case DRIVE_MOBIL_STATE:{
            if(accelerate){
                driver.drive(map(slider.get_state(), 1, range_of_slider,
                             static_cast<int>(driver.get_start_speed()),
                             static_cast<int>(driver.get_max_speed())));

            } else {
                driver.not_accelerate_drive();

            }
            stripLight.display_number(slider.get_state(),range_of_slider);
            //stripLight.display_number(static_cast<int>(driver),range_of_slider);
            back_tuner.stop();
            if(drive && ! reverse){
                machine_state = DRIVE_MOBIL_STATE;
            } else if ( reverse && !drive) {
                machine_state = REVERSE_MOBIL_STATE;
            } else if (!reverse && !drive) {
                machine_state = NEUTRAL_MOBIL_STATE;
            }
        } break;
        case REVERSE_MOBIL_STATE:{
            if(accelerate){
                driver.reverse(map(slider.get_state(), 1, range_of_slider, static_cast<int>(driver.get_start_speed()),
                                   static_cast<int>(driver.get_max_speed())));
            } else {
                driver.not_accelerate_reverse();
            }
            stripLight.display_number(slider.get_state(),range_of_slider);
            back_tuner.play();
            if(drive && ! reverse){
                machine_state = DRIVE_MOBIL_STATE;
            } else if ( reverse && !drive) {
                machine_state = REVERSE_MOBIL_STATE;
            } else if (!reverse && !drive) {
                machine_state = NEUTRAL_MOBIL_STATE;
            }
        } break;
        case NEUTRAL_MOBIL_STATE:{
            if(accelerate){
                driver.neutral();
            }else{
                driver.neutral();
                driver.set_new_speed(driver.get_stop_speed());
            }
            stripLight.display_number(slider.get_state(),range_of_slider);
            back_tuner.stop();
            if(drive && ! reverse){
                machine_state = DRIVE_MOBIL_STATE;
            } else if ( reverse && !drive) {
                machine_state = REVERSE_MOBIL_STATE;
            } else if (!reverse && !drive) {
                machine_state = NEUTRAL_MOBIL_STATE;
            }
        } break;
        default:{
            machine_state = LOAD_MOBIL_STATE;
        }
    }

}
#endif //ARDUINO_MOBIL_DESKTOP_H
