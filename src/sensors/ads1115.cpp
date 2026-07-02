#include "sensors/ads1115.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "config/pins.h"
#include "config/constants.h"
#include <Arduino.h>

static Adafruit_ADS1115 ads_1; 
static Adafruit_ADS1115 ads_2; 

// utils function
static float read_voltage_1(uint8_t channel, int samples = 20) {
    long sum = 0;
    for (size_t i = 0; i < samples; i++)
    {
        sum += ads_1.readADC_SingleEnded(channel);
        delay(5);
    }
    float avg_raw = (float) sum / samples;
    return avg_raw * constant::ADS_BIT_MV / 1000.0f;  
} 

static float read_voltage_2(uint8_t channel, int samples = 20) {
    long sum = 0;
    for (size_t i = 0; i < samples; i++)
    {
        sum += ads_2.readADC_SingleEnded(channel);
        delay(5);
    }
    float avg_raw = (float) sum / samples;
    return avg_raw * constant::ADS_BIT_MV / 1000.0f;  
} 

namespace ads1115 {
    bool initialize() {
        bool success = true;
        
        // Initialize I2C
        Wire.begin(SDA_PIN, SCL_PIN);

        // Initialize Module 1
        if (!ads_1.begin(constant::ADS1115_ADDRESS_1)) {
            Serial.println("Error: ADS1115 Module 1 (0x48) not found.");
            success = false;
        } else {
            Serial.println("Success: ADS115 Module 1 found");
            ads_1.setGain(GAIN_ONE);
        }

        // Initialize Module 2
        if (!ads_2.begin(constant::ADS1115_ADDRESS_2)) {
            Serial.println("Error: ADS1115 Module 2 (0x49) not found.");
            success = false;
        } else {
            Serial.println("Success: ADS115 Module 2 found");
            ads_2.setGain(GAIN_ONE);
        }

        return success;
    }

    float get_voltage_value_1(int index) {
        // Validation to prevent out-of-bounds index
        if (index < 0 || index > 3) return 0.0f;
        // int16_t raw = ads_1.readADC_SingleEnded(index);
        // return ads_1.computeVolts(raw);
        return read_voltage_1(index);
    }

    float get_voltage_value_2(int index) {
        if (index < 0 || index > 3) return 0.0f;
        // int16_t raw = ads_2.readADC_SingleEnded(index);
        // return ads_2.computeVolts(raw);
        return read_voltage_2(index);
    }
}