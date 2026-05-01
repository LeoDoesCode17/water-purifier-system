#ifndef NETWORKS_NETWORK_MANAGER_H
#define NETWORKS__NETWORK_MANAGER_H
#include "config/types.h"
namespace network_manager {
    void initialize();
    bool is_wifi_connected();
    void reconnect_wifi();
    bool mqtt_connect();
    void mqtt_loop();
    bool is_mqtt_connected();
    bool mqtt_publish_sensor_data(WaterQuality water_quality);
    bool mqtt_publish_status();
}
#endif