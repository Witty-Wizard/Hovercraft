#ifndef SBUS_H
#define SBUS_H
#include <Arduino.h>
#include "sbus_protocol.h"

class sbus{

private:
    int tx_pin;
    int rx_pin;

    #ifdef ESP32
    HardwareSerial *sbus_port;

    #elif defined(ARDUINO_ARCH_RP2040)
    SerialUART *sbus_port;
    #endif

    bool inverted;
    bool header_detected_sbus = false;
    int prev_buffer_sbus;
    int buffer_sbus;
    int rx_index;
    uint8_t data_rx[25];
    
public:

    #ifdef ESP32
    sbus(HardwareSerial *sbus_port, int rx_pin, int tx_pin, bool inverted = true);
    #elif defined(ARDUINO_ARCH_RP2040)
    sbus(SerialUART *port, int rx = 1, int tx = 0);
    #endif

    void init();
    void read(sbuspacket_t* data);
    void write(sbuspacket_t* data);

};

#endif