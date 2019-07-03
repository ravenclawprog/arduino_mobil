#include <Arduino.h>
#include "led_array.h"
#include "music_box.h"
int buzzer_pin = 5;                                // пин бузера
int led_pin[] = {6,7,8,9,10,11,12,13,A0};          // пины светодиодов
// мелодия
int melody[] = {NOTE_G4, NOTE_C5, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_C4};
// длительность
int duration[] = {8,4,12,16,4,8,8,4,12,16,4,4};
LED_array stripLight(&led_pin[0],sizeof(led_pin)/sizeof(int));
music_box tuner(buzzer_pin, &melody[0],&duration[0],sizeof(melody)/sizeof(int),700);
bool stop_led = false;
bool stop_tune= false;
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  if(!stop_led){
    stripLight.operate(LED_array::SALUTE_STATE);    
  }

  if(!stop_tune){
      tuner.play();  
  }
  tuner.operate();
  if(stripLight.get_state() == LED_array::IDLE_STATE){
    stop_led = true;    
  }
  if(!tuner.isPlay()){
    stop_tune = true; 
  }
  
}
