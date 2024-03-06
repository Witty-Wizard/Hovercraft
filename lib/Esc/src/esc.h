#ifndef ESC_H
#define ESC_H
#include <Arduino.h>

#define MODE_3D true

enum protocol_motor{
    PWM,
    FAST_PWM,
    ONESHOT125,
    ONESHOT42,
    MULTISHOT
};

class esc{

private:
    int channel;
    int frequency;
    int pin;
    int min_value;
    int mid_value;
    int max_value;
    bool mode;
    protocol_motor protocol_t;

public:

    esc(int pin, protocol_motor protocol_t, bool mode = false);

    void init();
    void write(float value);
};

#endif