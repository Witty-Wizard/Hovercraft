#ifndef SBUS_H
#define SBUS_H
#include <Arduino.h>
#include "sbus_protocol.h"

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

#ifdef ESP32
void init_sbus(HardwareSerial *sbus_port, int rx_pin, int tx_pin, bool inverted = true);
#elif defined(ARDUINO_ARCH_RP2040)
void init_sbus(SerialUART *port, int rx_pin = 1, int tx_pin = 0);
#endif


void read(sbuspacket_t* data);
void write(sbuspacket_t* data);

#endif