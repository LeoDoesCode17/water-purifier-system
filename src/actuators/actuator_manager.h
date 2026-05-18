#ifndef ACTUATORS_ACTUATOR_MANAGER_H
#define ACTUATORS_ACTUATOR_MANAGER_H
namespace actuator_manager {
    void initialize();
    void openServoValve();
    void closeServoValve();
    void turnOnGearMotor();
    void turnOffGearMotor();
    void turn_on_pump_1();
    void turn_on_pump_2();
    void turn_off_pump_1();
    void turn_off_pump_2();
}
#endif