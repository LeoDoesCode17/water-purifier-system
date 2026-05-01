#include "networks/mqtt.h"
#include "config/secrets.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <Arduino.h>

static WiFiClient espClient;
static PubSubClient mqttClient(espClient);

static void callback(char* topic, byte* payload, unsigned int length) {
    Serial.println("\n========== MQTT MESSAGE RECEIVED ==========");
    Serial.print("[MQTT] Topic   : ");
    Serial.println(topic);

    String message;
    for (unsigned int i = 0; i < length; i++) {
      message += (char)payload[i];
    }

    Serial.print("[MQTT] Payload : ");
    Serial.println(message);
    Serial.println("===========================================\n");
}

namespace mqtt {
    void initialize() {
      mqttClient.setServer(secret::MQTT_BROKER, secret::MQTT_PORT);
      mqttClient.setCallback(callback);
    }

    bool connect() {
        Serial.print("[MQTT] Connecting to broker: ");
        Serial.println(secret::MQTT_BROKER);

        bool connected;

        if (strlen(secret::MQTT_USERNAME) > 0) {
            connected = mqttClient.connect(secret::MQTT_CLIENT_ID, secret::MQTT_USERNAME, secret::MQTT_PASSWORD);
        } else {
            connected = mqttClient.connect(secret::MQTT_CLIENT_ID);
        }

        if (connected) {
            Serial.println("[MQTT] Connected");

            if (mqttClient.subscribe(secret::MQTT_SUB_TOPIC)) {
            Serial.print("[MQTT] Subscribed to: ");
            Serial.println(secret::MQTT_SUB_TOPIC);
            } else {
            Serial.println("[MQTT] Subscribe failed");
            }
        } else {
            Serial.print("[MQTT] Failed, rc = ");
            Serial.println(mqttClient.state());
        }

        return connected;
    }

    void loop() {
        mqttClient.loop();
    }

    bool is_connected() {
        return mqttClient.connected();
    }

    bool publish(const char *topic, const uint8_t *payload, unsigned int plength) {
        return mqttClient.publish(topic, payload, plength);
    }


}