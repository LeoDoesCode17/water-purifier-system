#include "actuators/gear_motor.h"
#include "config/pins.h"
#include <Arduino.h>
namespace gear_motor {
    void initialize() {
        pinMode(GEAR_MOTOR_PIN, OUTPUT);
    }
    void on() {
        digitalWrite(GEAR_MOTOR_PIN, HIGH);
    }
    void off() {
        digitalWrite(GEAR_MOTOR_PIN, LOW);
    }
}