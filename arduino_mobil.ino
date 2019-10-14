const int level_speed = A7; //потенциометр орграничителя скорости (принимаем от него значение при считывании 0-1023)

int speed ; //переменная хранения значения потенциометра 0-1023
int real_speed ; //переменная хранения значения 60-255 пропорцианализированная из speed
int output_speed=0 ; //переменная хранения значения 60-255 выдаваемого на шим регулирование на базе real_speed
bool first_loop = true;

int accelerate = A1; //педаль газа (в нажатом положении LOW)


const int drive = A3; //положение рычага КПП (в положении ЕХАТЬ ВПЕРЕД LOW)
const int revers = A4; //положение рычага КПП (в положении ЕХАТЬ НАЗАД LOW)
//если же drive и revers оба в положении HIGH, то это положение НЕЙТРАЛЬ

const int L_PWM = 2; //один сигнальный пин драйвера двигателя (если подать на него High то это будет плюс, если Low то минус)
const int PWM = 5; //пин R_EN и пин L_EN  сюда выдаем значение скорости (ШИМ 0-255)
const int R_PWM = 4; //второй сигнальный пин драйвера двигателя (если подать на него High то это будет плюс, если Low то минус)

const int buzzer = 6;//пищалка 

const int ledCount = 9;    // число светодиодов на светодиодной шкале
int ledPins[] = { 3, 7, 8, 9, 10, 11, 12, 13, 14, 15 };   // массив портов, к которым привязаны светодиод
const int stepOfSpeed = 1;                                // шаг, на который увеличивается скорость, чтобы достигнуть real_speed
int setNewSpeed(int& output_speed_, int real_speed_, int PWM_, int step_of_speed_){
    int sub = output_speed_ - real_speed_;
    if(sub > 0){
        output_speed_ -= step_of_speed_;
    } else if (sub < 0){
        output_speed_ += step_of_speed_;
    } else if (sub < step_of_speed_){
        output_speed_ = real_speed_;
    }
   // if(output_speed_ < 60) {
     //   output_speed_ = 60;
   // } else 
    if(output_speed_ > 255){
        output_speed_ = 255;
    }
   // analogWrite(PWM_, output_speed_);
}
void setup()  { 
    for (int thisLed = 0; thisLed < ledCount; thisLed++) {
        pinMode(ledPins[thisLed], OUTPUT);
    }
    Serial.begin(9600);

    pinMode(level_speed, INPUT);

    pinMode(accelerate, INPUT);
    digitalWrite(accelerate, HIGH );

    pinMode(drive, INPUT);
    digitalWrite(drive, HIGH );
    pinMode(revers, INPUT);
    digitalWrite(revers, HIGH );

    pinMode(L_PWM, OUTPUT);
    digitalWrite(L_PWM, HIGH );
    pinMode(R_PWM, OUTPUT);
    digitalWrite(R_PWM, LOW );

    pinMode(PWM, OUTPUT);

    for (int thisLed = 0; thisLed < ledCount; thisLed++) {
        digitalWrite(ledPins[thisLed], HIGH);
        delay(100);
        //  digitalWrite(ledPins[thisLed], LOW);
    }
    // loop from the highest pin to the lowest:
    // for (int thisLed = ledCount - 1; thisLed >= 0; thisLed++) {
    for (int thisLed = 0; thisLed < ledCount; thisLed++) {
        // digitalWrite(ledPins[thisLed], HIGH);
        delay(100);
        digitalWrite(ledPins[thisLed], LOW);
    }

} 
/// Основной рабочий цикл 
void loop()  { 

    if (digitalRead(drive) == LOW & digitalRead(revers) == HIGH){
        digitalWrite(L_PWM, LOW );
        digitalWrite(R_PWM, HIGH );
    }
    if (digitalRead(drive) == HIGH & digitalRead(revers) == LOW){
        digitalWrite(L_PWM, HIGH );
        digitalWrite(R_PWM, LOW );
    }
    if (digitalRead(drive) == HIGH & digitalRead(revers) == HIGH){
        digitalWrite(L_PWM, LOW );
        digitalWrite(R_PWM, LOW );
    }


    if (digitalRead(accelerate) == LOW) {
        if (first_loop) {
            output_speed = 60;
            first_loop = false;
        }


        int speed = analogRead(level_speed);
        int ledLevel = map(speed, 0, 1015, 1, ledCount);

        int real_speed = map(speed, 0, 1024, 60, 255);
        setNewSpeed(output_speed,real_speed,PWM,stepOfSpeed);
        analogWrite(PWM, output_speed);
        Serial.print(" | output_speed:   ");
        Serial.println(output_speed);      // Выводим значение в Serial Monitor
        delay(100);

        for (int thisLed = 0; thisLed < ledCount; thisLed++) {
            // если индекс элемента массива меньше чем ledLevel,
            // включаем порт для данного элемента:
            if (thisLed < ledLevel) {
                digitalWrite(ledPins[thisLed], HIGH);
            }
            // Выключаем все порты, которые выше чем ledLevel:
            else {
                digitalWrite(ledPins[thisLed], LOW);
            }

        }
    }



    if (digitalRead(accelerate) == HIGH) {
      first_loop = true;
        analogWrite(PWM, 0);
        Serial.print(" | real_speed: ");
        Serial.println(real_speed);

        int speed = analogRead(level_speed);
        int real_speed = map(speed, 0, 1024, 60, 255);
        setNewSpeed(output_speed,0,PWM,stepOfSpeed);
        Serial.print(" | real_speed:   ");
        Serial.println(real_speed);      // Выводим значение в Serial Monitor
        delay(100);

        int ledLevel = map(speed, 0, 1015, 1, ledCount);
        // Подаем наше трансформированное значение на вывод светодиода LED_PIN
        for (int thisLed = 0; thisLed < ledCount; thisLed++) {
            // если индекс элемента массива меньше чем ledLevel,
            // включаем порт для данного элемента:
            if (thisLed < ledLevel) {
                digitalWrite(ledPins[thisLed], HIGH);
            }
            // Выключаем все порты, которые выше чем ledLevel:
            else {
                digitalWrite(ledPins[thisLed], LOW);
            }

        }
    }
}
