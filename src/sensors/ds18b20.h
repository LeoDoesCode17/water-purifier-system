#ifndef SENSORS_DS18B20_H
#define SENSORS_DS18B20_H
namespace ds18b20 {
    bool initialize();
    float get_temperature_c_1();
    float get_temperature_c_2();
    int get_device_disconnected();
    void update();
}
#endif