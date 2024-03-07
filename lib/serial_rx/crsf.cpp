#include "crsf.h"

#if defined(ESP32)
void init_crsf(HardwareSerial *crsf_port, int rx_pin, int tx_pin, bool inverted = false){
    crsf_port->begin(CRSF_BAUDRATE, SERIAL_8N1, rx_pin, tx_pin, inverted);
}

#elif defined(ARDUINO_ARCH_RP2040)
void init_crsf(HardwareSerial *crsf_port, int rx_pin, int tx_pin, bool inverted = false){
    crsf_port->setRX(rx_pin);
    crsf_port->setTX(tx_pin);
    crsf_port->begin(CRSF_BAUDRATE, SERIAL_8N1);
}

#elif defined(STM32F4xx)
void init_crsf(){
    crsf_port->begin(CRSF_BAUDRATE, SERIAL_8N1);
}
#endif

void read(){
    while (crsf_port->available()){
        uint8_t buffer = crsf_port->read();
        if (header_detected){
            // rx_index = (rx_index + 1) % (rx_data[1] + 1);
            rx_data[rx_index] = buffer;
            rx_index++;
            if (rx_index > rx_data[1]+2){
                rx_index = 0;
                header_detected = false;
            }
        }
        else{
            if (buffer == CRSF_ADDRESS_FLIGHT_CONTROLLER || buffer == CRSF_ADDRESS_CRSF_TRANSMITTER){
                header_detected = true;
                rx_data[0] = buffer;
                rx_index = 1;
            }
        }

        if (rx_index == sizeof(rx_data) / sizeof(rx_data[0])){
            // if rx_data buffer overflow
            rx_index = 0;
            header_detected = false;
        }
    }
        memcpy(&header, rx_data, sizeof(header));
        
        if (header.device_addr == CRSF_ADDRESS_FLIGHT_CONTROLLER){

            if (header.type == CRSF_FRAMETYPE_RC_CHANNELS_PACKED){
                memcpy(&packet, rx_data + 3, sizeof(packet));
            }
            else if (header.type == CRSF_FRAMETYPE_LINK_STATISTICS){
                memcpy(&link_status, rx_data + 3, sizeof(link_status));
            }

        }
        else if (header.device_addr == CRSF_ADDRESS_CRSF_TRANSMITTER){
            if (header.type == CRSF_FRAMETYPE_RC_CHANNELS_PACKED){
                memcpy(&packet, rx_data + 3, sizeof(packet));
            }
        }
    
}

crsf_channels_t getChannel(){
    return packet;
}

crsfLinkStatistics_t getlinkStatus(){
    return link_status;
}
