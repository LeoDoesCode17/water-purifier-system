#ifndef SENSORS_ULTRASONIC_H
#define SENSORS_ULTRASONIC_H
namespace ultrasonic {
    void initialize();
    void trigger_sensor_1_high();
    void trigger_sensor_2_high();
    void trigger_sensor_1_low();
    void trigger_sensor_2_low();
    int get_echo_sensor_1();
    int get_echo_sensor_2();
}
#endif