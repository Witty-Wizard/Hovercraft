#ifndef SERIAL_RX_H
#define SERIAL_RX_H
#include <Arduino.h>
#include "../sbus/src/sbus.h"
#include "../crsf/src/crsf.h"

#define INVERTED true
#define UNINVERTED false

enum protocol{
    SBUS,
    CRSF
};

class serial_rx{
    private:
        HardwareSerial *serial_port;
        protocol protocol_t;
        int rx_pin;
        int tx_pin;
        bool inverted;

        serial_rx(protocol protocol_t,HardwareSerial *serial_port,int rx_pin, int tx_pin,bool inverted = false);
    
    public:
        void init();
};


#endif
