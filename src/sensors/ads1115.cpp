#include "sensors/ads1115.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "config/pins.h"
#include "config/constants.h"

static Adafruit_ADS1115 ads_1; 
static Adafruit_ADS1115 ads_2; 

namespace ads1115 {
    bool initialize() {
        bool success = true;
        
        // Initialize I2C
        Wire.begin(pins::SDA_PIN, pins::SCL_PIN);

        // Initialize Module 1
        if (!ads_1.begin(constant::ADS1115_ADDRESS_1)) {
            Serial.println("Error: ADS1115 Module 1 (0x48) not found.");
            success = false;
        } else {
            ads_1.setGain(GAIN_ONE);
        }

        // Initialize Module 2
        if (!ads_2.begin(constant::ADS1115_ADDRESS_2)) {
            Serial.println("Error: ADS1115 Module 2 (0x49) not found.");
            success = false;
        } else {
            ads_2.setGain(GAIN_ONE);
        }

        return success;
    }

    float get_voltage_value_1(int index) {
        // Validation to prevent out-of-bounds index
        if (index < 0 || index > 3) return 0.0f;
        int16_t raw = ads_1.readADC_SingleEnded(index);
        return ads_1.computeVolts(raw);
    }

    float get_voltage_value_2(int index) {
        if (index < 0 || index > 3) return 0.0f;
        int16_t raw = ads_2.readADC_SingleEnded(index);
        return ads_2.computeVolts(raw);
    }
    
    // You can also add a helper for raw values if needed
    int16_t get_raw_value_1(int index) {
        return ads_1.readADC_SingleEnded(index);
    }
}