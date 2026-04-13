#include "sensors/ds18b20.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "config/pins.h"
#include "config/secrets.h"

static OneWire one_wire(pins::DQ_PIN);
static DallasTemperature ds_sensor(&one_wire);

namespace ds18b20 {
    bool initialize() {
        ds_sensor.begin();
        return ds_sensor.getDeviceCount() > 0;
    }

    void update() {
        ds_sensor.requestTemperatures();
    }

    float get_temperature_c_1() {
        return ds_sensor.getTempC(secret::DS18B20_ADDRESS_1);
    }

    float get_temperature_c_2() {
        return ds_sensor.getTempC(secret::DS18B20_ADDRESS_2);
    }

    int get_device_disconnected() {
        return DEVICE_DISCONNECTED_C;
    }
}