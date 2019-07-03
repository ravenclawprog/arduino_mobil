#ifndef LED_ARRAY_H
#define LED_ARRAY_H

/// Класс массив светодиодов
/// Позволяет управлять группой светодиодов
class LED_array {

public:
    enum LED_array_state{
        UNDEFINED_STATE = 0,                     // неопределенный режим
        IDLE_STATE,                              // режим ожидания - в данном режиме происходит отображение текущего состояния массива states_ в соответствующие пины.
        SALUTE_STATE,                            // режим анимации - задаёт приветственную анимацию
        DISPLAY_STATE                            // данный режим производит отображение заднного числа из заданного дипазона на соответствующие диапазон имеющихся светодиодов
    };
    LED_array();                           // конструктор по умолчанию - создает массив светодиодов нулевой длины - лучше не использовать
    LED_array(int *pins, int a_length, bool reverse_logic = false, unsigned long time_between_light = 500); // основной конструктор массива светодиодов - на вход подаётся ссылка на массив пинов, длина данного массива и (необязательно) режим обратной логики
    ~LED_array();                          // удаляет выделенные раннее динамические структуры
    void erase();                          // удаляет все созданные ранее данные
    //int& operator[](int index);          // перегрузка оператора индексации - вернуть значение массива pins_
    bool &operator[](int pin);             // перегрузка оператора индексации - вернуть значение массива states_ в соответствии с pin, который содержится в массиве pins_
    int get_length();                      // метод возвращает длину массива светодиодов
    void set_reverse_logic();              // метод устанавливает работу в обратной логике
    void unset_reverse_logic();            // метод устаналвивает работу в прямой логике
    void write_by_pin(int pin, bool state);// метод записывает в соответствующий pin соответствующее значение state
    void write_by_index(int index, bool state);// метод записывает по соответствующему index'у в массиве pins соответствующее значение state
    void write_all();                      // записывает при помощи метода digitalWrite значения для всех
    LED_array_state operate(LED_array_state l_state = LED_array::UNDEFINED_STATE, int DISPLAY_STATE_number = 0, int range = -1);
	LED_array_state get_state();
private:
    int  a_length_;                     // длина массива
    int  *pins_;                        // массив пинов
    bool *states_;                      // массив состояний пинов
    bool reverse_logic_;                // обратная логика
    int pin_in_pins(int pin);           // функция определяет, содержится ли число pin в массиве pins. Если да, то возвращает индекс в массиве pins, иначе - -1
    LED_array(int a_length);            // конструктор, который создает массив светодиодов на основе  только длины
    unsigned long time_between_light_;  // время в мс между включением/отключением лампочек в состоянии SALUTE_STATE
    LED_array_state l_state_;           // состояние массива светодиодов

    //// переменные для стартовой анимации

    bool end_SALUTE_STATE_;                   // признак завершения анимации
    bool first_loop_;                   // признак первой итерации анимации
    unsigned long last_time_;           // переменная для сохранения времени
    int counter_;                       // счетчик включенных/отключенных лампочек
    bool straight_animation_;           // признак прямого прохода

};

int LED_array::pin_in_pins(int pin)
{
    bool pin_in_pins_ = false;
    int index = 0;
    for(int i = 0; i < a_length_; i++){
        if(pin == pins_[i]){
            pin_in_pins_ = true;
            index = i;
            break;
        }
    }
    if(pin_in_pins_) return index;
    else             return -1;
}

LED_array::LED_array()
{
    l_state_  = LED_array::UNDEFINED_STATE;
    a_length_ = 0;
    pins_     = NULL;
    states_   = NULL;
    reverse_logic_ = false;
}

LED_array::LED_array(int a_length)
{
    l_state_  = LED_array::UNDEFINED_STATE;
    reverse_logic_ = false;
    if(a_length > 0){
        a_length_ = a_length;
        pins_     = new int[a_length_];
        states_   = new bool[a_length_];
    } else {
        a_length_ = 0;
        pins_     = NULL;
        states_   = NULL;
    }

}

LED_array::LED_array(int *pins, int a_length, bool reverse_logic, unsigned long time_between_light)
{
    time_between_light_ = time_between_light;
    reverse_logic_ = reverse_logic;
    if(a_length > 0){
        a_length_ = a_length;
        pins_     = new int[a_length_];
        states_   = new bool[a_length_];
        for(int i = 0; i < a_length_; i++){
            pins_[i]   = pins[i];
            states_[i] = false;
            pinMode(pins_[i], OUTPUT);
            write_by_index(i,false);
        }
    } else {
        a_length_ = 0;
        pins_     = NULL;
        states_   = NULL;
    }

}

LED_array::~LED_array()
{
    if(pins_){
        delete [] pins_;
    }
    if(states_){
        delete [] states_;
    }
}

void LED_array::erase()
{
    if(pins_) {
        delete [] pins_;
    }
    if(states_) {
        delete [] states_;
    }
    a_length_ = 0;
    pins_     = NULL;
    states_   = NULL;

}

bool &LED_array::operator[](int pin)
{
    int index = pin_in_pins(pin);
    if(index != -1){
        return states_[index];
    } else {
        return states_[0];
    }
}

int LED_array::get_length()
{
    return a_length_;
}

void LED_array::set_reverse_logic()
{
    reverse_logic_ = true;
}

void LED_array::unset_reverse_logic()
{
    reverse_logic_ = false;
}

void LED_array::write_by_pin(int pin, bool state)
{
    int index = pin_in_pins(pin);
    if(index != -1){
        digitalWrite(pins_[index], state  ? (reverse_logic_ ? LOW : HIGH )
                                          : (reverse_logic_ ? HIGH : LOW ));
        states_[index] = state;
    }
}

void LED_array::write_by_index(int index, bool state)
{
    if(index >=0 && index < a_length_){
        digitalWrite(pins_[index], state  ? (reverse_logic_ ? LOW : HIGH )
                                          : (reverse_logic_ ? HIGH : LOW ));
        states_[index] = state;
    }
}

void LED_array::write_all()
{
    for(int i = 0; i < a_length_; i++){
        digitalWrite(pins_[i], states_[i]  ? (reverse_logic_ ? LOW : HIGH )
                                           : (reverse_logic_ ? HIGH : LOW ));
    }
}

LED_array::LED_array_state LED_array::operate(LED_array::LED_array_state l_state, int DISPLAY_STATE_number, int range)
{

    // исполнение задач в определенных состояниях

    switch(l_state_) {
        case LED_array::UNDEFINED_STATE:{
            write_all();
            last_time_ = millis();
            end_SALUTE_STATE_ = false;
            first_loop_ = true;
            counter_ = 0;
            straight_animation_ = true;
        } break;
        case LED_array::IDLE_STATE:{
            write_all();
            last_time_ = millis();
            end_SALUTE_STATE_ = false;
            first_loop_ = true;
            counter_ = 0;
            straight_animation_ = true;
        } break;
        case LED_array::SALUTE_STATE:{
            if(first_loop_){
                last_time_ = millis();
                end_SALUTE_STATE_ = false;
                first_loop_ = false;
                counter_ = 0;
                straight_animation_ = true;
                for(int i = 0; i < a_length_; i++){
                    states_[i] = false;
                }
                write_all();
            }

            if(millis() - last_time_ >= time_between_light_ && !end_SALUTE_STATE_) {

                // прямой проход

                if(straight_animation_){
                    write_by_index(counter_,true);
                    counter_++;
                    if(counter_ >= a_length_) {
                        counter_ = a_length_ - 1;
                        straight_animation_ = false;
                    }

                // обратный проход

                } else {
                    write_by_index(counter_,false);
                    counter_--;
                    if(counter_ <= 0) {
                        counter_ = 0;
                        straight_animation_ = true;
                        end_SALUTE_STATE_ = true;
                    }
                }

                last_time_ = millis();
            }
        } break;
        case LED_array::DISPLAY_STATE:{

            // отображение, когда диапазон не задан

            if(range = -1){
                for(int i = 0; i < a_length_; i++){
                    if(i <= DISPLAY_STATE_number) {
                        states_[i] = true;
                    } else {
                        states_[i] = false;
                    }
                }
                write_all();
            // отображение, когда дипазон задан
            } else {
                if(range > 0){
                    int l = (a_length_ * DISPLAY_STATE_number) / range;
                    for(int i = 0; i < a_length_; i++){
                        if(i <= l) {
                            states_[i] = true;
                        } else {
                            states_[i] = false;
                        }
                    }
                    write_all();
                }
            }
            end_SALUTE_STATE_ = false;
            first_loop_ = true;
            counter_ = 0;
            straight_animation_ = true;
        } break;
        default: {
            end_SALUTE_STATE_ = false;
            first_loop_ = true;
            counter_ = 0;
            straight_animation_ = true;
        } break;
    }

    // переходы между состояниями

    switch(l_state_) {
        case LED_array::UNDEFINED_STATE:{
            l_state_ = l_state;
        } break;
        case LED_array::IDLE_STATE:{
            l_state_ = l_state;
        } break;
        case LED_array::SALUTE_STATE:{
            if(end_SALUTE_STATE_) l_state_ = LED_array::IDLE_STATE;
        } break;
        case LED_array::DISPLAY_STATE:{
            l_state_ = l_state;
        } break;
        default: {
            l_state_ = LED_array::UNDEFINED_STATE;
        } break;
    }

    return l_state_;
}

LED_array::LED_array_state LED_array::get_state(){
	return l_state_;
}

#endif // LED_ARRAY_H
