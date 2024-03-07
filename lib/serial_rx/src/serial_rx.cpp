#pragma once
#include "serial_rx.h"

serial_rx::serial_rx(protocol protocol_t,HardwareSerial *serial_port,int rx_pin, int tx_pin,bool inverted){
    this->serial_port = serial_port;
    this->protocol_t = protocol_t;
    this->rx_pin = rx_pin;
    this->tx_pin = tx_pin;
    this->inverted = inverted;
}

void serial_rx::init(){
    if(protocol_t == SBUS){
        init_sbus(serial_port,rx_pin,tx_pin,inverted);
    }
    else if(protocol_t == CRSF){
        
    }
    else{
        log_e("invalid protocol");
    }
}