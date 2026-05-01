#include "networks/network_manager.h"
#include "networks/wifi.h"
#include "networks/mqtt.h"
#include "config/secrets.h"
#include <time.h>
#include <ArduinoJson.h>  
#include <time.h>

static void init_time() {
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    
    Serial.print("[TIME] Syncing");
    time_t now = time(nullptr);
    int retry = 0;
    while (now <= 100000 && retry < 20) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
        retry++;
    }
    Serial.println();

    if (now > 100000) {
        Serial.println("[TIME] Time synchronized");
    } else {
        Serial.println("[TIME] Time sync failed");
    }
}

static bool get_iso8601_utc(char* out, size_t len) {
    time_t now = time(nullptr);
    if (now <= 100000) return false;

    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    return strftime(out, len, "%Y-%m-%dT%H:%M:%SZ", &timeinfo) > 0;
}

namespace network_manager {
    void initialize() {
        wifi::initialize();
        mqtt::initialize();
        init_time();
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

    bool is_mqtt_connected() {
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
        // const size_t DOC_CAPACITY = JSON_OBJECT_SIZE(4) + 128;
        // const unsigned DOC_CAPACITY = 256;
        // StaticJsonDocument<DOC_CAPACITY> doc;   
        JsonDocument doc;   
        char recorded_at[25];

        if (!get_iso8601_utc(recorded_at, sizeof(recorded_at))) {
            strcpy(recorded_at, "1970-01-01T00:00:00Z");
        }
        
        doc["mac_address"] = "00:1A:2B:3C:4D:5E";
        doc["status"] = "online";
        doc["message"] = "ESP Online";
        doc["recorded_at"] = recorded_at;

        char buffer[256];
        
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