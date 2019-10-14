#ifndef __PIN_H
#define __PIN_H
enum MODE{
    OUTPUT,
    INPUT,
    INPUT_PULLUP
};

template <class T>
class __pin
{
public:
    enum mode_of_pin{
        output_mode = 0,
        input_mode,
        input_pullup_mode
    };

    //__pin(T state, mode_of_pin m = input_mode);
    __pin();
    T read();
    void write(T w);
private:
    T state_;
    mode_of_pin m_;
};


template<class T>
__pin<T>::__pin()
{ }

template<class T>
T __pin<T>::read()
{
    return state_;
}

template<class T>
void __pin<T>::write(T w)
{
    state_ = w;
}

#endif // __PIN_H
