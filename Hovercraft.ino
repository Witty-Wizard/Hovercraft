/*
*Robotics Pec
*robotics@pec.edu.in
*
*                    GNU GENERAL PUBLIC LICENSE
*                       Version 3, 29 June 2007
*
*Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
*Everyone is permitted to copy and distribute verbatim copies
*of this license document, but changing it is not allowed.
*
*The GNU General Public License is a free, copyleft license for
*software and other kinds of works.
*
*THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY
*APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT
*HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY
*OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
*THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
*PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM
*IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF
*ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
*/

#include "src/sbus/sbus.h"
#include <Servo.h>

Servo myservo

/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial2,16,17,true);
/* SBUS data */
bfs::SbusData data;

Servo inflate;

void setup() {
  inflate.attach(23);
  Serial.begin(115200);
  while (!Serial) {}
  /* Begin the SBUS communication */
  sbus_rx.Begin();
}

void loop () {
  if (sbus_rx.Read()){
    /* Grab the received data */
    data = sbus_rx.data();
    inflate.write(map(data.ch[2],173,1180,0,180));
    /* Display the received data */
    for (int8_t i = 0; i < 4; i++) {
      Serial.print(data.ch[i]);
      Serial.print("\t");
    }
    /* Display lost frames and failsafe data */
    Serial.print(data.lost_frame);
    Serial.print("\t");
    Serial.println(data.failsafe);
  }
}

int throttle=0;

void throttle(){
  
  data = sbus_rx.data();

  throttle.write(map(data.ch[7], 0,180)); // mapped the data from 0,180
  
}

int leftThrust = 0;

void leftThrust(){
  data = sbus_rx.data();

  leftThrust.write(map(data.ch[8], 0, 180));


  
  
}


int rightThrust = 0;

void rightThrust(){
  data = sbus_rx.data();

  rightThrust.write(map(data.ch[9],0, 180));
}
