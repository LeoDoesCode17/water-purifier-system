#include "actuators/pump.h"
#include "config/pins.h"
#include <Arduino.h>

namespace pump {
    void initialize() {
        pinMode(pins::PUMP_PIN, OUTPUT);
    }

    void on() {
        digitalWrite(pins::PUMP_PIN, HIGH);
    }

    void off() {
        digitalWrite(pins::PUMP_PIN, LOW);
    }
}