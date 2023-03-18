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

#include "src/sbus/sbus.h"

#define ROLL 0
#define ELEVATOR 1
#define THROTTLE 2
#define ARM 4

#define INFLATE 1
#define THRUSTER 2
#define RUDDER 3

#define INFLATE_PIN 23
#define THRUSTER_PIN 25
#define RUDDER_PIN 21

#define PWM_FREQUENCY 50
#define PWM_RESOLUTION 12
#define PWM_MIN 206
#define PWM_MAX 406
#define PWM_MID 306

#define SBUS_RX 16
#define SBUS_TX 17


/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial2, SBUS_RX, SBUS_TX, true);
/* SBUS data */
bfs::SbusData data;

int throttle_val = PWM_MIN;
int thrust_val = PWM_MIN;
int rudder_val = PWM_MID;

int max_ch3 = 1811;
int max_ch2 = 1811;
int max_ch1 = 1811;
int max_ch0 = 1748;

int min_ch3 = 172;
int min_ch2 = 172;
int min_ch1 = 172;
int min_ch0 = 241;


void setup() {
  //Setup Infaltion motor on pin 23
  ledcSetup(INFLATE,PWM_FREQUENCY,PWM_RESOLUTION);
  ledcAttachPin(INFLATE_PIN,INFLATE);

  //Setup Thruster motor on pin 25
  ledcSetup(THRUSTER,PWM_FREQUENCY,PWM_RESOLUTION);
  ledcAttachPin(THRUSTER_PIN,THRUSTER);

  //Setup Rudder servo on pin 21
  ledcSetup(RUDDER,PWM_FREQUENCY,PWM_RESOLUTION);
  ledcAttachPin(RUDDER_PIN,RUDDER);

  //Debugging
  Serial.begin(115200);
  while (!Serial) {}

  //Begin Sbus communication
  sbus_rx.Begin();
}

void loop () {
  if (sbus_rx.Read()) {
    data = sbus_rx.data();

    // ARM on channel 5
    if ((data.ch[ARM] < 1000) || data.failsafe ){
      throttle_val = PWM_MIN;
      thrust_val = PWM_MIN;
      rudder_val = PWM_MID;
      
    }
    else {
      if(!data.lost_frame){
        throttle_val = map(constrain(data.ch[THROTTLE], min_ch2 , max_ch2), min_ch2, max_ch2, PWM_MIN, PWM_MAX);
        thrust_val = map(constrain(data.ch[ELEVATOR], min_ch0, max_ch0), min_ch0, max_ch0, PWM_MIN, PWM_MAX);
        rudder_val = map(constrain(data.ch[ROLL], min_ch0, max_ch0), min_ch0, max_ch0, PWM_MIN, PWM_MAX);
      }
    }

    ledcWrite(INFLATE,throttle_val);
    ledcWrite(THRUSTER,thrust_val);
    ledcWrite(RUDDER, rudder_val);


//debugging
    for (int8_t i = 0; i < 3; i++) {
      Serial.print(data.ch[i]);
      Serial.print("\t");
    }

    Serial.print(data.lost_frame);
    Serial.print("\t");
    Serial.println(data.failsafe);
  }
}

void calibrate(){
  Serial.println("Rotate sticks in all directions");
  int max_ch3 = 1000 , max_ch2 = 1000 , max_ch1 = 1000 , max_ch0 = 1000;
  int min_ch3 = 1000 , min_ch2 = 1000 , min_ch1 = 1000 , min_ch0 = 1000;

  unsigned t = millis();
  while (true and millis() - t < 10000)
  {
    if (sbus_rx.Read()) {
      data = sbus_rx.data();
      Serial.println("Receiving data");
      max_ch0 = data.ch[0] > max_ch0 ? data.ch[0] : max_ch0;
      max_ch1 = data.ch[1] > max_ch1 ? data.ch[1] : max_ch1;
      max_ch2 = data.ch[2] > max_ch2 ? data.ch[2] : max_ch2;
      max_ch3 = data.ch[3] > max_ch3 ? data.ch[3] : max_ch3;

      min_ch0 = data.ch[0] < min_ch0 ? data.ch[0] : min_ch0;
      min_ch1 = data.ch[1] < min_ch1 ? data.ch[1] : min_ch1;
      min_ch2 = data.ch[2] < min_ch2 ? data.ch[2] : min_ch2;
      min_ch3 = data.ch[3] < min_ch3 ? data.ch[3] : min_ch3;
    }

  }
  Serial.println("Calibration done paste this in code");
  Serial.print("int max_ch3 = "); Serial.print(max_ch3); Serial.println(";");
  Serial.print("int max_ch2 = "); Serial.print(max_ch2); Serial.println(";");
  Serial.print("int max_ch1 = "); Serial.print(max_ch1); Serial.println(";");
  Serial.print("int max_ch0 = "); Serial.print(max_ch0); Serial.println(";");

  Serial.print("int min_ch3 = "); Serial.print(min_ch3); Serial.println(";");
  Serial.print("int min_ch2 = "); Serial.print(min_ch2); Serial.println(";");
  Serial.print("int min_ch1 = "); Serial.print(min_ch1); Serial.println(";");
  Serial.print("int min_ch0 = "); Serial.print(min_ch0); Serial.println(";");
}
