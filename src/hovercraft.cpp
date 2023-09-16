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

#include <Arduino.h>
#include <crsf.h>
#include <sbus.h>
#include <esc.h>

// filter coefficient
#define alpha 0.5

// define motor pins here
#define INFLATE_PIN 5
#define THRUST_LEFT_PIN 18
#define THRUST_RIGHT_PIN 16

// motor signal values
#define MOTOR_MIN 0
#define MOTOR_MID 512
#define MOTOR_MAX 1023
#define DEADZONE 60

// uncomment to use sbus
sbuspacket_t packet;
sbus reciever(&Serial1,17,16);

// uncomment to use crsf
// crsf_channels_t packet;
// crsf reciever(&Serial1,16,17);

// configure motors with Oneshot125
esc inflate(INFLATE_PIN,ONESHOT125);
esc thrust_left(THRUST_LEFT_PIN,ONESHOT125);
esc thrust_right(THRUST_RIGHT_PIN,ONESHOT125);

// variable to store the motor throttle value
float inflate_value = MOTOR_MIN;
float left_thrust = MOTOR_MID;
float right_thrust = MOTOR_MID;
// variable for filtering
float prev_inflate_value = MOTOR_MIN;
float prev_left_thrust = MOTOR_MID;
float prev_right_thrust = MOTOR_MID;

// variable to store incoming values from transmitter in 10bit 
float throttle_value = MOTOR_MIN;
float elevator_value = MOTOR_MID;
float aileron_value = MOTOR_MID;

float forward_value = MOTOR_MID;
float turning_value = MOTOR_MIN;

void setup(){
  // initialize Serial port for debugging
  Serial.begin(115200);
  
  // initialize the sbus pin
  reciever.init();

  // initialize motors
  inflate.init();
  thrust_left.init();
  thrust_right.init();
}

void loop(){
  // read the sbus packet and store in a struct called packet
  reciever.read(&packet);

  // mapping the recieved values to 10bit values
  aileron_value = constrain(map(packet.channel1,172,1811,-1023,1023),-1023,1023);
  elevator_value = constrain(map(packet.channel2,172,1811,0,1023),0,1023);
  throttle_value = constrain(map(packet.channel3,172,1811,0,1023),0,1023);

  if((elevator_value <= MOTOR_MID - DEADZONE) || (elevator_value >= MOTOR_MID + DEADZONE)){
    forward_value = elevator_value;
  }
  else{
    forward_value = MOTOR_MID;
  }

  if((aileron_value <= MOTOR_MIN - DEADZONE) || (aileron_value >= MOTOR_MIN + DEADZONE)){
    turning_value = aileron_value;
  }
  else{
    turning_value = MOTOR_MIN;
  }

  // mix the elevator and aileron channels
  right_thrust = elevator_value - turning_value;
  left_thrust = elevator_value + turning_value;
  inflate_value = throttle_value;

  // constraining the value
  right_thrust = constrain(right_thrust,0,1023);
  left_thrust = constrain(left_thrust,0,1023);
  inflate_value = constrain(inflate_value,0,1023);

  right_thrust = alpha * right_thrust + (1 - alpha) * prev_right_thrust;
  left_thrust = alpha * left_thrust + (1 - alpha) * prev_left_thrust;
  inflate_value = alpha * inflate_value + (1 - alpha) * prev_inflate_value;

  prev_right_thrust = right_thrust;
  prev_left_thrust = left_thrust;
  prev_inflate_value = inflate_value;

  // write the throttle values to motors
  inflate.write(inflate_value);
  thrust_left.write(left_thrust);
  thrust_right.write(right_thrust);
  
  // uncomment for debugging
  // Serial.print(inflate_value);
  // Serial.print("  ");
  // Serial.print(left_thrust);
  // Serial.print("  ");
  // Serial.print(right_thrust);
  // Serial.println("  ");
}
