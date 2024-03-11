#pragma once
#include "sbus.h"

#ifdef ESP32
void init_sbus(HardwareSerial *sbus_port, int rx_pin, int tx_pin,
               bool inverted = true) {
  sbus_port->begin(BAUDRATE_SBUS, SERIAL_8E2, rx_pin, tx_pin, inverted);
}
#elif defined(ARDUINO_ARCH_RP2040)
void init_sbus(SerialUART *port, int rx_pin = 1, int tx_pin = 0) {
  sbus_port->setTX(tx_pin);
  sbus_port->setRX(rx_pin);
  sbus_port->begin(BAUDRATE_SBUS, SERIAL_8E2);
}
#endif

void read(sbuspacket_t *data) {

  while (sbus_port->available()) {
    prev_buffer_sbus = buffer_sbus;
    buffer_sbus = sbus_port->read();

    if (header_detected_sbus == true) {
      data_rx[rx_index] = buffer_sbus;
      rx_index++;
      if (rx_index > 23) {
        header_detected_sbus = false;
      }
    } else {
      if (prev_buffer_sbus == FOOTER_SBUS && buffer_sbus == HEADER_SBUS) {
        header_detected_sbus = true;
        data_rx[0] = 0x0F;
        data_rx[24] = 0x00;
        rx_index = 1;
      }
    }

    if (rx_index == sizeof(data_rx) / sizeof(data_rx[0])) {
      rx_index = 0;
      header_detected_sbus = false;
    }
  }
  memcpy(data, data_rx, sizeof(*data));
}

void write(sbuspacket_t *data) {
  sbus_port->write((char *)data, sizeof(*data));
}