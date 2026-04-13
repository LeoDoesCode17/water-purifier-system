#ifndef SENSORS_ADS1115_H
#define SENSORS_ADS1115_H
#include <stdint.h>
namespace ads1115 {
    void initialize();
    int16_t get_adc_value(int index);
    float get_voltage_value(int index);
}
#endif