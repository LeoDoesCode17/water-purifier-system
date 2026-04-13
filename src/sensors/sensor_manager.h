#ifndef SENSORS_SENSOR_MANAGER_H
#define SENSORS_SENSOR_MANAGER_H
namespace sensor_manager {
    void initialize_all();
    float get_distance_ultrasonic_1();
    float get_distance_ultrasonic_2();
    float get_temperature_c_1();
    float get_temperature_c_2();
    float get_ph_value_1();
    float get_ph_value_2();
    float get_turbidity_ntu_value_1();
    float get_turbidity_ntu_value_2();
    float get_tds_ppm_value_1();
    float get_tds_ppm_value_2();
    void update_all();
}
#endif