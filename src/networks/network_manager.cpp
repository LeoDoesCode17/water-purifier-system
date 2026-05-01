#include "networks/network_manager.h"
#include "networks/wifi.h"
#include "networks/mqtt.h"
#include "config/secrets.h"
#include <time.h>
#include <ArduinoJson.h>  

namespace network_manager {
    void initialize() {
        wifi::initialize();
        mqtt::initialize();
    }

    bool is_wifi_connected() {
        return wifi::is_connected();
    }

    void reconnect_wifi() {
        wifi::initialize();
    }
    
    bool mqtt_connect() {
        return mqtt::connect();
    }

    void mqtt_loop() {
        mqtt::loop();
    }

    bool mqtt_is_connected() {
        return mqtt::is_connected();
    }

    bool mqtt_publish_sensor_data(WaterQuality water_quality) {
        StaticJsonDocument<256> doc;
        doc["raw_tds"] = water_quality.ppm;
        doc["raw_turbidity"] = water_quality.ntu;
        doc["raw_temperature"] = water_quality.temperature;
        doc["raw_ph"] = water_quality.ph;
        doc["raw_water_level"] = water_quality.level;

        char buffer[256];
        size_t len = serializeJson(doc, buffer);

        bool ok = mqtt::publish(secret::MQTT_PUB_TOPIC, (uint8_t*)buffer, len);
        if (ok) {
            Serial.print("[MQTT] Published: ");
            Serial.println(buffer);
        } else {
            Serial.println("[MQTT] Publish failed");
        }
        return ok;
    }

    bool mqtt_publish_status() {
        const unsigned long N = 256;
        StaticJsonDocument<N> doc;
        doc["mac_address"] = "00:1A:2B:3C:4D:5E";
        doc["status"] = "online";
        doc["message"] = "ESP Online";
        doc["recorded_at"] = "2026-04-28T10:00:00Z";

        char buffer[N];
        size_t len = serializeJson(doc, buffer);
        bool ok = mqtt::publish(secret::MQTT_DEVICE_STATUS_PUB_TOPIC, (uint8_t*)buffer, len);

        if (ok) {
            Serial.print("[MQTT] Published: ");
            Serial.println(buffer);
        } else {
            Serial.println("[MQTT] Publish failed");
        }
        return ok;
    }
}