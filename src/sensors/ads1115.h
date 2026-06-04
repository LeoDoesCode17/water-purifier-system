#ifndef SENSORS_ADS1115_H
#define SENSORS_ADS1115_H
namespace ads1115 {
    bool initialize();
    float get_voltage_value_1(int index);
    float get_voltage_value_2(int index);
}
#endif