#include <Arduino.h>
#include<esp32-hal-rmt.h>

#define DSHOT_BITS 16

#define MOTOR_PIN   21 
rmt_obj_t* rmt = NULL;
// 11 bit throttle: 2048 possible values. 0 is reserved for disarmed. 1-47 are reserved for special commands. Leaving 48 to 2047 (2000 steps) for the actual throttle value
// 1 bit telemetry request - if this is set, telemetry data is sent back via a separate channel
// 4 bit CRC: (Cyclic Redundancy) Check to validate data (throttle and telemetry request bit)
// --------------------------------------------------------------
// | DShot | Bitrate 	| T1H	| T0H	| Bit (µs) | Frame (µs) |
// | 150   | 150kbit/s	| 5.00	| 2.50	| 6.67     | 106.72     |
// | 300   | 300kbit/s	| 2.50	| 1.25	| 3.33	   | 53.28      |
// | 600   | 600kbit/s	| 1.25	| 0.625	| 1.67	   | 26.72      |
// | 1200  | 1200kbit/s | 0.625 | 0.313 | 0.83	   | 13.28      |
// --------------------------------------------------------------
//

//
//      Bits encoded as pulses as follows:
//
//      "0":
//         +-------+              +--
//         |       |              |
//         |       |              |
//         |       |              |
//      ---|       |--------------|
//         +       +              +
//         |  T0H  |      T0L     |
//
//      "1":
//         +-------------+       +--
//         |             |       |
//         |             |       |
//         |             |       |
//         |             |       |
//      ---+             +-------+
//         |     T1H     |  T1L  |

rmt_data_t dshot_bit_0;
rmt_data_t dshot_bit_1;

void setup() {
    Serial.begin(115200);

    // dshot 300
    dshot_bit_0.level0 = 1;
    dshot_bit_0.duration0 = 2.5;
    dshot_bit_0.level1 = 0;
    dshot_bit_0.duration1 = 0.83;

    dshot_bit_1.level0 = 1;
    dshot_bit_1.duration0 = 1.25;
    dshot_bit_1.level1 = 0;
    dshot_bit_1.duration1 = 2.08;
    
    rmt = rmtInit(MOTOR_PIN, RMT_TX_MODE,RMT_MEM_64);
}


void loop() {
    // Send the data and wait until it is done
    rmtWrite(rmt, &dshot_bit_0, DSHOT_BITS);
    delay(100);
}