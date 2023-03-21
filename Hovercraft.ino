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
#define RUDDER 2
#define ARM 4

#define INFLATE_PWM_CH 1
#define THRUSTERC_PWM_CH 2
#define RUDDERC_PWM_CH 3

#define INFLATE_PIN 19
#define THRUSTER_PIN 18
#define RUDDER_PIN 4



/////////////////////////////////////PWM Config
#define PWM_FREQUENCY 50
#define PWM_RESOLUTION 12
#define PWM_MIN 206 // corresponds to 0 degress
#define PWM_MAX 406 // corresponds to 180 degrees
#define PWM_MID 306 // corresponds to 90 degrees

#define INFLATE_PWM_MAX 370
#define THRUSTER_PWM_MAX 370

///////////////////////////////////LOW pass filter config / integrator

#define total_number_of_samples 200
int thruster_average[total_number_of_samples] = {0};
int inflate_average[total_number_of_samples] = {0};


// Sbus config

#define SBUS_RX 16
#define SBUS_TX 17


/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial2, SBUS_RX, SBUS_TX, true);
/* SBUS data */
bfs::SbusData data;

float throttle_val = PWM_MIN;
float thrust_val = PWM_MIN;
int rudder_val = PWM_MID;



/////////////////////////////////////////////////User Specified Data /////////////////////////////////////////////
int max_ch_rudder = 1811;
int max_ch_throttle = 1811;
int max_ch_elevator = 1240;
int max_ch_aileron = 1748;

int min_ch_rudder = 172;
int min_ch_throttle = 172;
int min_ch_elevator = 1024;
int min_ch_aileron = 241;


void setup() {
  //Setup Infaltion motor on pin 23
  ledcSetup(INFLATE_PWM_CH, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(INFLATE_PIN, INFLATE_PWM_CH);

  //Setup Thruster motor on pin 25
  ledcSetup(THRUSTERC_PWM_CH, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(THRUSTER_PIN, THRUSTERC_PWM_CH);

  //Setup Rudder servo on pin 21
  ledcSetup(RUDDERC_PWM_CH, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(RUDDER_PIN, RUDDERC_PWM_CH);

  //Debugging
  Serial.begin(115200);
  while (!Serial) {}

  //Begin Sbus communication
  sbus_rx.Begin();
}

// add failsafe time of 2 sec

void loop () {
  if (sbus_rx.Read()) {
    data = sbus_rx.data();

    for (int i = total_number_of_samples - 1; i > 0; i--) {
      // shifting of values
      inflate_average[i] = inflate_average[i - 1];
      thruster_average[i] = thruster_average[i - 1];


    }
    // ARM on channel 5
    if ((data.ch[ARM] < 1000) || data.failsafe ) {
      throttle_val = PWM_MIN;
      thrust_val = PWM_MIN;
      rudder_val = PWM_MID;

    }
    else {
      if (!data.lost_frame) {
        // added low pass filter so ESC doesnt blow up because of high currents
        inflate_average[0] = map(constrain(data.ch[THROTTLE], min_ch_throttle , max_ch_throttle), min_ch_throttle, max_ch_throttle, PWM_MIN, INFLATE_PWM_MAX);
        thruster_average[0] = map(constrain(data.ch[ELEVATOR], min_ch_aileron, max_ch_aileron), min_ch_aileron, max_ch_aileron, PWM_MIN, THRUSTER_PWM_MAX);

        rudder_val = map(constrain(data.ch[ROLL], min_ch_aileron, max_ch_aileron), min_ch_aileron, max_ch_aileron, PWM_MIN, PWM_MAX);



      }
    }

    // compute average

    for (int i = 0; i < total_number_of_samples ; i++) {
      // shifting of values
      throttle_val += inflate_average[i]/total_number_of_samples;
      thrust_val += thruster_average[i]/total_number_of_samples;


    }
    // write values to motors
    ledcWrite(INFLATE_PWM_CH, throttle_val);
    ledcWrite(THRUSTERC_PWM_CH, thrust_val);
    ledcWrite(RUDDERC_PWM_CH, rudder_val);


    //debugging
    for (int8_t i = 0; i < 3; i++) {
      Serial.print(data.ch[i]);
      Serial.print("\t");
    }

    Serial.print(data.lost_frame);
    Serial.print("\t");
    Serial.print(thrust_val);
    Serial.print("\t");
    Serial.println(data.failsafe);
  }
}

void calibrate() {
  Serial.println("Rotate sticks in all directions");
  int max_ch_rudder = 1000 , max_ch_throttle = 1000 , max_ch_elevator = 1000 , max_ch_aileron = 1000;
  int min_ch_rudder = 1000 , min_ch_throttle = 1000 , min_ch_elevator = 1000 , min_ch_aileron = 1000;

  unsigned t = millis();
  while (true and millis() - t < 10000)
  {
    if (sbus_rx.Read()) {
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
  Serial.print("int max_ch_rudder = "); Serial.print(max_ch_rudder); Serial.println(";");
  Serial.print("int max_ch_throttle = "); Serial.print(max_ch_throttle); Serial.println(";");
  Serial.print("int max_ch_elevator = "); Serial.print(max_ch_elevator); Serial.println(";");
  Serial.print("int max_ch_aileron = "); Serial.print(max_ch_aileron); Serial.println(";");

  Serial.print("int min_ch_rudder = "); Serial.print(min_ch_rudder); Serial.println(";");
  Serial.print("int min_ch_throttle = "); Serial.print(min_ch_throttle); Serial.println(";");
  Serial.print("int min_ch_elevator = "); Serial.print(min_ch_elevator); Serial.println(";");
  Serial.print("int min_ch_aileron = "); Serial.print(min_ch_aileron); Serial.println(";");
}
