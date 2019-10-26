#ifndef ARDUINOUNO_H
#define ARDUINOUNO_H
#include <arduinomachine.h>

class ArduinoUno
{
private:
    ArduinoUno() {}
    ~ArduinoUno() {}
     ArduinoUno(const ArduinoUno&);                 // Prevent copy-construction
     ArduinoUno& operator=(const ArduinoUno&);      // Prevent assignment
public:
    static ArduinoMachine& getInstance() {
        static ArduinoMachine instance;
        return instance;
    }
};

#endif // ARDUINOUNO_H
