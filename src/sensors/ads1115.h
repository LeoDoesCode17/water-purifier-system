#ifndef SENSORS_ADS1115_H
#define SENSORS_ADS1115_H
#include <stdint.h>
namespace ads1115 {
    bool initialize();
    int16_t get_adc_value_1(int index);
    float get_voltage_value_1(int index);
    int16_t get_adc_value_2(int index);
    float get_voltage_value_2(int index);
}
#endif