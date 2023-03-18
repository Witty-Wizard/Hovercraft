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

#define INFLATE 1

/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial2, 16, 17, true);
/* SBUS data */
bfs::SbusData data;

int throttle_val = 0;
int right_val = 0;
int left_val = 0;
int steering = 0;


int max_ch3 = 1811;
int max_ch2 = 1811;
int max_ch1 = 1811;
int max_ch0 = 1748;

int min_ch3 = 172;
int min_ch2 = 172;
int min_ch1 = 172;
int min_ch0 = 241;


void setup() {
  ledcSetup(INFLATE,50,12);

  ledcAttachPin(23,INFLATE);
  Serial.begin(115200);
  while (!Serial) {}
  sbus_rx.Begin();
//  calibrate();
//  delay(5000);
}

void loop () {
  if (sbus_rx.Read()) {
    /* Grab the received data */
    data = sbus_rx.data();

    throttle_val = map(constrain(data.ch[2], min_ch2 , max_ch2), min_ch2, max_ch2, 205, 406);
    steering = map(data.ch[0], 0, 2047, 0, 180);


    if (steering > 90) {
      right_val = throttle_val * (180 - steering) / 90;
      left_val = throttle_val;
    }
    else {
      left_val = throttle_val * steering / 90;
      right_val = throttle_val;
    }

    // ARM on channel 5
    if (data.ch[4] > 1000) {
      ledcWrite(INFLATE,throttle_val);
    }
    else {
      throttle_val = 205;
      ledcWrite(INFLATE,throttle_val);
    }



    /* Display the received data */
    for (int8_t i = 2; i < 3; i++) {
      Serial.print(data.ch[i]);
      Serial.print("\t");
    }
    Serial.println(throttle_val);
    /* Display lost frames and failsafe data */
    
//    Serial.print(right_val);
//    Serial.print("\t");
//    Serial.print(left_val);
//    Serial.print("\t");
//    Serial.print(data.lost_frame);
//    Serial.print("\t");
//    Serial.println(data.failsafe);
  }
}

void calibrate()
{
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

void normalize()
{

}
