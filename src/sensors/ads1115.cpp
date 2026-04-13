#include "sensors/ads1115.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "config/pins.h"

static Adafruit_ADS1115 ads;

namespace ads1115 {
    void initialize() {
        Wire.begin(pins::SDA_PIN, pins::SCL_PIN);
        if (!ads.begin()) {
            Serial.println("Failed to initialize ads1115");
        }
        ads.setGain(GAIN_ONE); // gain one for range sensor up to 4.096volt
    }

    int16_t get_adc_value(int index) {
        return ads.readADC_SingleEnded(index);
    }

    float get_voltage_value(int index) {
        int16_t voltage = ads.readADC_SingleEnded(index);
        return ads.computeVolts(voltage);
    }
}