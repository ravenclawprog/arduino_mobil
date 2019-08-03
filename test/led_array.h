#ifndef LED_ARRAY_H
#define LED_ARRAY_H

#include <Arduino.h>

/// Класс массив светодиодов
/// Позволяет управлять группой светодиодов
///
enum LED_array_state{
    UNDEFINED_STATE = 0,                            // неопределенный режим
    IDLE_STATE,                                     // режим ожидания - в данном режиме происходит отображение текущего состояния массива states_ в соответствующие пины.
    SALUTE_STATE,                                   // режим анимации - задаёт приветственную анимацию
    DISPLAY_STATE                                   // данный режим производит отображение заднного числа из заданного дипазона на соответствующие диапазон имеющихся светодиодов
};
template <size_t n_>
class LED_array {
public:

    LED_array();                                        // конструктор по умолчанию - создает массив светодиодов нулевой длины - лучше не использовать
    LED_array(int (&pins)[n_], bool reverse_logic = false, unsigned long time_between_light = 500); // основной конструктор массива светодиодов - на вход подаётся ссылка на массив пинов, длина данного массива и (необязательно) режим обратной логики
    ~LED_array();                                       // удаляет выделенные раннее динамические структуры
    int get_length();                                   // метод возвращает длину массива светодиодов
    void set_reverse_logic();                           // метод устанавливает работу в обратной логике
    void reset_reverse_logic();                         // метод устаналвивает работу в прямой логике
    void write_by_pin(int pin, bool state);             // метод записывает в соответствующий pin соответствующее значение state
    void write_by_index(int index, bool state);         // метод записывает по соответствующему index'у в массиве pins соответствующее значение state
    void write_all();                                   // записывает при помощи метода digitalWrite значения для всех
    bool is_salute_end();
    LED_array_state get_state();
    void salute();
    void display_number(int number, int range = -1);
    void idle();
private:
    size_t a_length_;                    // длина массива
    int*   pins_;                        // массив пинов
    bool   states_[n_];                      // массив состояний пинов
    bool   reverse_logic_;               // обратная логика
    LED_array_state l_state_;            // состояние массива светодиодов

    int pin_in_pins(int pin);            // функция определяет, содержится ли число pin в массиве pins. Если да, то возвращает индекс в массиве pins, иначе - -1
    void init();                         // функция инициализирует начальные значения переменных
    //// переменные для стартовой анимации

    bool end_salute_;                    // признак завершения анимации
    bool first_loop_;                    // признак первой итерации анимации
    unsigned long last_time_;            // переменная для сохранения времени
    int counter_;                        // счетчик включенных/отключенных лампочек при анимации
    bool straight_animation_;            // признак прямого прохода
    unsigned long time_between_light_;   // время в мс между включением/отключением лампочек в состоянии SALUTE_STATE

};
template<size_t n_>
int LED_array<n_>::pin_in_pins(int pin)
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
template<size_t n_>
void LED_array<n_>::init()
{
    l_state_  = UNDEFINED_STATE;
    a_length_ = 0;
    pins_     = NULL;
    for (int i = 0;i < n_; i++) {
        states_[i] = false;
    }
    reverse_logic_ = false;
}

template<size_t n_>
LED_array<n_>::LED_array()
{
    init();
}
template<size_t n_>
LED_array<n_>::LED_array(int (&pins)[n_], bool reverse_logic, unsigned long time_between_light)
{
    time_between_light_ = time_between_light;
    reverse_logic_ = reverse_logic;
    a_length_ = sizeof(pins)/sizeof(pins[0]);
    pins_     = pins;
    for(int i = 0; i < a_length_; i++){
            states_[i] = false;
            pinMode(pins_[i], OUTPUT);
            write_by_index(i,false);
    }
    l_state_ = IDLE_STATE;
}
template<size_t n_>
LED_array<n_>::~LED_array()
{
    for(int i = 0; i < a_length_; i++){
            states_[i] = false;
            write_by_index(i,false);
    }
}
template<size_t n_>
int LED_array<n_>::get_length()
{
    return a_length_;
}
template<size_t n_>
void LED_array<n_>::set_reverse_logic()
{
    reverse_logic_ = true;
}
template<size_t n_>
void LED_array<n_>::reset_reverse_logic()
{
    reverse_logic_ = false;
}
template<size_t n_>
void LED_array<n_>::write_by_pin(int pin, bool state)
{
    int index = pin_in_pins(pin);
    if(index != -1){
        digitalWrite(pins_[index], state  ? (reverse_logic_ ? LOW : HIGH )
                                          : (reverse_logic_ ? HIGH : LOW ));
        states_[index] = state;
    }
}
template<size_t n_>
void LED_array<n_>::write_by_index(int index, bool state)
{
    if(index >=0 && index < a_length_){
        digitalWrite(pins_[index], state  ? (reverse_logic_ ? LOW : HIGH )
                                          : (reverse_logic_ ? HIGH : LOW ));
        states_[index] = state;
    }
}
template<size_t n_>
void LED_array<n_>::write_all()
{
    for(int i = 0; i < a_length_; i++){
        digitalWrite(pins_[i], states_[i]  ? (reverse_logic_ ? LOW : HIGH )
                                           : (reverse_logic_ ? HIGH : LOW ));
    }
}
template<size_t n_>
bool LED_array<n_>::is_salute_end()
{
    return end_salute_;
}
template<size_t n_>
LED_array_state LED_array<n_>::get_state(){
    return l_state_;
}
template<size_t n_>
void LED_array<n_>::salute()
{
    l_state_ = SALUTE_STATE;
    if(first_loop_) {
        last_time_ = millis();
        end_salute_ = false;
        first_loop_ = false;
        counter_ = 0;
        straight_animation_ = true;
        for(int i = 0; i < a_length_; i++){
            states_[i] = false;
        }
        write_all();
    }

    if(millis() - last_time_ >= time_between_light_ && !end_salute_) {

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
            if(counter_ < 0) {
                counter_ = 0;
                straight_animation_ = true;
                end_salute_ = true;
            }
        }

        last_time_ = millis();
    }
}
template<size_t n_>
void LED_array<n_>::display_number(int number, int range)
{
    l_state_ = DISPLAY_STATE;
    // отображение, когда диапазон не задан

    if(range <= -1){
        for(int i = 0; i < a_length_; i++){
            if(i <= number) {
                states_[i] = true;
            } else {
                states_[i] = false;
            }
        }
        //write_all();
    // отображение, когда дипазон задан
    } else {
        if(range > 0){
            int l = map(number, 0, range, 1, a_length_);
            for(int i = 0; i < a_length_; i++){
                if(i < l) {
                    states_[i] = true;
                } else {
                    states_[i] = false;
                }
            }
            //write_all();
        }
    }
    idle();
}
template<size_t n_>
void LED_array<n_>::idle()
{
    write_all();
    last_time_ = millis();
    end_salute_ = false;
    first_loop_ = true;
    counter_ = 0;
    straight_animation_ = true;
}
#endif // LED_ARRAY_H
