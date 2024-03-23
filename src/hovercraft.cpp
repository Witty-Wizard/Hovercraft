/**
 * @file main.cpp
 * @brief Main application file.
 * @author Robotics Society
 * @date March 11, 2024
 *
 * @license GNU General Public License Version 3
 */

#include <Arduino.h>
#include <serialIO.h>
#include <DriveMaster.h>

crsf_channels_t channelData;

serialIO *receiver = NULL;
DriveMaster *motor = nullptr;

void setup() {
    Serial.begin(115200);
    receiver = new crsf(Serial1,5,-1);
    receiver->begin();
    motor = new dshot(5);
    motor->begin();
}

void loop() {
    receiver->processIncoming();
    receiver->getChannel(&channelData);
    Serial.println(channelData.channel1);
    motor->write();
}