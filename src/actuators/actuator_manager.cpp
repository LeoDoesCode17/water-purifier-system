#include "actuators/actuator_manager.h"
#include "actuators/servo_valve.h"
#include "actuators/gear_motor.h"
#include "actuators/pump.h"

namespace actuator_manager {
    void initialize() {
        servo_valve::initialize();
        gear_motor::initialize();
        pump::initialize();
    }

    void openServoValve() {
        servo_valve::open();
    }

    void closeServoValve() {
        servo_valve::close();
    }

    void turnOnGearMotor() {
        gear_motor::on();
    }

    void turnOffGearMotor() {
        gear_motor::off();
    }

    void turn_on_pump_1() {
        pump::on_1();
    }
    void turn_on_pump_2() {
        pump::on_2();
    }
    void turn_off_pump_1() {
        pump::off_1();
    }
    void turn_off_pump_2() {
        pump::off_2();
    }
    float get_opening_servo_time_by_dose_ms(float moringa_dose) {
        return 0.0;
    }
}