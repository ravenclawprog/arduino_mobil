#ifndef ARDUINO_H
#define ARDUINO_H
/**/
#include "arduino_interfaces.h"
#include "__pin.h"
#include "arduinouno.h"

void pinMode(int pin, int mode)
{
    Q_UNUSED(pin);
    Q_UNUSED(mode);//
}
bool digitalRead(int pin){
    return ArduinoUno::getInstance().digital_read(pin);
}
void digitalWrite(int pin, bool state){
    ArduinoUno::getInstance().digital_write(pin,state);
}
int  analogRead(int pin)
{
    return ArduinoUno::getInstance().analog_read(pin);
}
void analogWrite(int pin, int pwm)
{
   ArduinoUno::getInstance().analog_write(pin,pwm);
}
void noTone(int pin){
    ArduinoUno::getInstance().analog_write(pin,0);
};
void tone(int pin, int freq){
    ArduinoUno::getInstance().analog_write(pin,freq);
}
unsigned long millis(){
    return ArduinoUno::getInstance().millis();
}
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif // ARDUINO_H
