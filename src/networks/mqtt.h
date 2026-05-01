#ifndef NETWORKS_MQTT_H
#define NETWORKS_MQTT_H
namespace mqtt {
    void initialize();
    bool connect();
    void loop();
    bool is_connected();
    bool publish(const char *topic, const uint8_t *payload, unsigned int plength);
}
#endif