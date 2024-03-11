#pragma once
#include "crsf.h"

void crsf::begin() {
  _rxPort->begin(CRSF_BAUDRATE, SERIAL_8E2, _rxPin, _txPin, _inverted);
}