#include "servo_valve.h"
#include <ESP32Servo.h>
#include "config/pins.h"

static Servo servoValve;
static constexpr int SERVO_OPEN_ANGLE = 180; // open 100%
static constexpr int SERVO_CLOSE_ANGLE = 90; // close 100%

namespace servo_valve
{
    void initialize()
    {
        servoValve.attach(pins::SERVO_VALVE_PIN, 500, 2500);
        servoValve.write(SERVO_CLOSE_ANGLE);
        delay(500);
    }

    void open()
    {
        servoValve.write(SERVO_OPEN_ANGLE);
    }

    void close()
    {
        servoValve.write(SERVO_CLOSE_ANGLE);
    }
}