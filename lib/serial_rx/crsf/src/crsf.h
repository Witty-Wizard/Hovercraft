#ifndef CRSF_H
#define CRSF_H
#include <Arduino.h>
#include "crsf_protocol.h"

bool header_detected;
uint8_t rx_index;
uint8_t rx_data[CRSF_MAX_PACKET_SIZE];

#if defined(ESP32)
HardwareSerial *crsf_port;

#elif defined(ARDUINO_ARCH_RP2040)
SerialUART *crsf_port;

#elif defined(STM32F4xx)
HardwareSerial *crsf_port;

#endif

crsf_channels_t packet;
crsfLinkStatistics_t link_status;
crsf_header_t header;

#ifdef ESP32
void init(HardwareSerial *crsf_port, int rx_pin, int tx_pin, bool inverted = false);

#elif defined(ARDUINO_ARCH_RP2040)
void init(SerialUART *crsf_port, int rx_pin, int tx_pin);

#endif

void read();

crsf_channels_t getChannel();
crsfLinkStatistics_t getlinkStatus();

#endif