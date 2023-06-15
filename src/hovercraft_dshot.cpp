/*
  Robotics Pec
  robotics@pec.edu.in

                     GNU GENERAL PUBLIC LICENSE
                        Version 3, 29 June 2007

  Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
  Everyone is permitted to copy and distribute verbatim copies
  of this license document, but changing it is not allowed.

  The GNU General Public License is a free, copyleft license for
  software and other kinds of works.

  THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY
  APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT
  HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY
  OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM
  IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF
  ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
*/

#include <sbus.h>
#include <Arduino.h>
#include <DShotRMT.h>

DShotRMT dshot_01(GPIO_NUM_18, RMT_CHANNEL_0);

#define ROLL 0
#define ELEVATOR 1
#define THROTTLE 2
#define RUDDER 3
#define ARM 4
#define REVERSE_CH 5
#define HIGH_RATE_SWITCH 6

#define INFLATE_PWM_CH 1
// #define THRUSTERC_PWM_CH 2
#define RUDDERC_PWM_CH 3

#define INFLATE_PIN 19
// #define THRUSTER_PIN 18
#define RUDDER_PIN 21

#define PWM_FREQUENCY 50
#define PWM_RESOLUTION 12
#define SERVO_PWM_MIN 112 // corresponds to 0 degress
#define SERVO_PWM_MAX 500 // corresponds to 180 degrees
#define SERVO_PWM_MID 306 // corresponds to 90 degrees

#define PWM_MIN 206 // corresponds to 0 degress
#define PWM_MAX 406 // corresponds to 180 degrees

#define SBUS_RX 16
#define SBUS_TX 17

/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial2, SBUS_RX, SBUS_TX, true);
/* SBUS data */
bfs::SbusData data;

int throttle_val = PWM_MIN;
// int thrust_val = PWM_MIN;
volatile uint16_t thrust_val = 0x30; // ...sending "48", the first throttle value
constexpr auto FAILSAVE_THROTTLE = 0x3E7;
int rudder_val = SERVO_PWM_MID;

/////////////////////////////////////////////////User Specified Data /////////////////////////////////////////////
int max_ch_rudder = 1645;
int max_ch_throttle = 1630;
int max_ch_elevator = 1749;
int max_ch_aileron = 1732;
int min_ch_rudder = 225;
int min_ch_throttle = 232;
int min_ch_elevator = 260;
int min_ch_aileron = 243;

#define REVERSED 0
#define NORMAL 1
#define NON_3D_MODE 2
int mode = NORMAL;

void setup()
{
    // Setup Infaltion motor on pin 23
    ledcSetup(INFLATE_PWM_CH, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(INFLATE_PIN, INFLATE_PWM_CH);

    // Setup Thruster motor on pin 25
    //    ledcSetup(THRUSTERC_PWM_CH, PWM_FREQUENCY, PWM_RESOLUTION);
    //    ledcAttachPin(THRUSTER_PIN, THRUSTERC_PWM_CH);

    // Setup Rudder servo on pin 21
    ledcSetup(RUDDERC_PWM_CH, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(RUDDER_PIN, RUDDERC_PWM_CH);

    // Debugging
    Serial.begin(115200);
    while (!Serial)
    {
    }

    // Begin Sbus communication
    sbus_rx.Begin();

    // Begin Dshot
    dshot_01.begin(DSHOT300);
    //    calibrate();
    delay(1000);

    for (int i = 0; i < 6; i++)
    {
        dshot_01.send_dshot_value(0xA);
        delay(3);
        Serial.println("3d mode on");
    }
    delay(3);
    for (int i = 0; i < 6; i++)
    {
        dshot_01.send_dshot_value(0xC);
        delay(40);
        Serial.println("3d mode on");
    }
    delay(40);
}

void loop()
{
    if (sbus_rx.Read())
    {
        data = sbus_rx.data();

        // ARM on channel 5
        if ((data.ch[ARM] < 1000) || data.failsafe)
        {
            throttle_val = PWM_MIN;
            thrust_val = 0x30;
            rudder_val = SERVO_PWM_MID;
        }
        else
        {
            if (data.ch[REVERSE_CH] > 1500)
            {
                mode = NON_3D_MODE;
            }
            else if (data.ch[REVERSE_CH] > 1000)
            {
                mode = REVERSED;
            }
            else
            {
                mode = NORMAL;
            }
            if (!data.lost_frame)
            {
                throttle_val = map(constrain(data.ch[THROTTLE], min_ch_throttle, max_ch_throttle), min_ch_throttle, max_ch_throttle, PWM_MIN, PWM_MAX);
                if (mode == NORMAL)
                {
                    thrust_val = constrain(data.ch[ELEVATOR], (min_ch_elevator + max_ch_elevator) / 2, max_ch_elevator);
                    thrust_val = map(thrust_val, (min_ch_elevator + max_ch_elevator) / 2, max_ch_elevator, 0x42E, 0x7FF);
                }
                else if (mode == REVERSED)
                {
                    thrust_val = constrain(data.ch[ELEVATOR], min_ch_elevator, max_ch_elevator);
                    if (thrust_val >= (min_ch_elevator + max_ch_elevator) / 2)
                    {
                        thrust_val = map(thrust_val, (min_ch_elevator + max_ch_elevator) / 2, max_ch_elevator, 0x42E, 0x7FF);
                    }
                    else
                    {
                        thrust_val = map(thrust_val, min_ch_elevator, (min_ch_elevator + max_ch_elevator) / 2, 0x3E8, 0x30);
                    }
                }
                else if (mode == NON_3D_MODE)
                {
                    thrust_val = constrain(data.ch[ELEVATOR], min_ch_elevator, max_ch_elevator);
                    if (thrust_val >= (min_ch_elevator + max_ch_elevator) / 2)
                    {
                        thrust_val = map(thrust_val, (min_ch_elevator + max_ch_elevator) / 2, max_ch_elevator, 0x30, 0x7FF);
                    }
                    else
                    {
                        thrust_val = 0x30;
                    }
                }
                if (data.ch[HIGH_RATE_SWITCH] > 1500)
                    rudder_val = map(constrain(data.ch[ROLL], min_ch_aileron, max_ch_aileron), min_ch_aileron, max_ch_aileron, SERVO_PWM_MIN, SERVO_PWM_MAX);

                else
                    rudder_val = map(constrain(data.ch[ROLL], min_ch_aileron, max_ch_aileron), min_ch_aileron, max_ch_aileron, PWM_MIN, PWM_MAX);
            }
        }

        // write values to motors
        ledcWrite(INFLATE_PWM_CH, throttle_val);
        // ledcWrite(THRUSTERC_PWM_CH,thrust_val);
        dshot_01.send_dshot_value(thrust_val);
        ledcWrite(RUDDERC_PWM_CH, rudder_val);

        // debugging
        for (int8_t i = 0; i < 5; i++)
        {
            Serial.print(data.ch[i]);
            Serial.print("\t");
        }

        Serial.print(data.lost_frame);
        Serial.print("\t");
        Serial.print(thrust_val);
        Serial.print("\t");
        Serial.println(data.failsafe);
    }
    delayMicroseconds(2);
}

void calibrate()
{
    Serial.println("Rotate sticks in all directions");
    int max_ch_rudder = 1000, max_ch_throttle = 1000, max_ch_elevator = 1000, max_ch_aileron = 1000;
    int min_ch_rudder = 1000, min_ch_throttle = 1000, min_ch_elevator = 1000, min_ch_aileron = 1000;

    unsigned t = millis();
    while (true and millis() - t < 10000)
    {
        if (sbus_rx.Read())
        {
            data = sbus_rx.data();
            Serial.println("Receiving data");
            max_ch_aileron = data.ch[ROLL] > max_ch_aileron ? data.ch[ROLL] : max_ch_aileron;
            max_ch_elevator = data.ch[ELEVATOR] > max_ch_elevator ? data.ch[ELEVATOR] : max_ch_elevator;
            max_ch_throttle = data.ch[THROTTLE] > max_ch_throttle ? data.ch[THROTTLE] : max_ch_throttle;
            max_ch_rudder = data.ch[RUDDER] > max_ch_rudder ? data.ch[RUDDER] : max_ch_rudder;

            min_ch_aileron = data.ch[ROLL] < min_ch_aileron ? data.ch[ROLL] : min_ch_aileron;
            min_ch_elevator = data.ch[ELEVATOR] < min_ch_elevator ? data.ch[ELEVATOR] : min_ch_elevator;
            min_ch_throttle = data.ch[THROTTLE] < min_ch_throttle ? data.ch[THROTTLE] : min_ch_throttle;
            min_ch_rudder = data.ch[RUDDER] < min_ch_rudder ? data.ch[RUDDER] : min_ch_rudder;
        }
    }
    Serial.println("Calibration done paste this in code");
    Serial.print("int max_ch_rudder = ");
    Serial.print(max_ch_rudder);
    Serial.println(";");
    Serial.print("int max_ch_throttle = ");
    Serial.print(max_ch_throttle);
    Serial.println(";");
    Serial.print("int max_ch_elevator = ");
    Serial.print(max_ch_elevator);
    Serial.println(";");
    Serial.print("int max_ch_aileron = ");
    Serial.print(max_ch_aileron);
    Serial.println(";");

    Serial.print("int min_ch_rudder = ");
    Serial.print(min_ch_rudder);
    Serial.println(";");
    Serial.print("int min_ch_throttle = ");
    Serial.print(min_ch_throttle);
    Serial.println(";");
    Serial.print("int min_ch_elevator = ");
    Serial.print(min_ch_elevator);
    Serial.println(";");
    Serial.print("int min_ch_aileron = ");
    Serial.print(min_ch_aileron);
    Serial.println(";");

    while (true)
        ;
}
