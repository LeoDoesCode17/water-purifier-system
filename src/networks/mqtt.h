#ifndef NETWORKS_MQTT_H
#define NETWORKS_MQTT_H
#include "config/types.h"
namespace mqtt {
    void initialize();
    void set_callback(std::function<void (char *, uint8_t *, unsigned int)> callback);
    bool connect();
    void loop();
    bool is_connected();
    bool publish(WaterQuality water_quality);
}
#endif