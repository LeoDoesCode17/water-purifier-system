#include "actuators/ball_valve.h"
#include <Arduino.h>
#include "config/pins.h"
namespace ball_valve {
    void initialize() {
        pinMode(pins::BALL_VALVE_PIN, OUTPUT);
    }

    void open() {
        digitalWrite(pins::BALL_VALVE_PIN, HIGH);
    }

    void close() {
        digitalWrite(pins::BALL_VALVE_PIN, LOW);
    }
}