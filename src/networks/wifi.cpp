#include "networks/wifi.h"
#include <WiFi.h>
#include "config/secrets.h"

namespace wifi {
    void initialize() {
        Serial.print("[WIFI] Connecting to ");
        Serial.println(secret::WIFI_SSID);

        WiFi.begin(secret::WIFI_SSID, secret::WIFI_PASSWORD);

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }

        Serial.println();
        Serial.println("[WIFI] Connected");
        Serial.print("[WIFI] IP Address: ");
        Serial.println(WiFi.localIP());
    }

    bool is_connected() {
        return WiFi.status() == WL_CONNECTED;
    }
}