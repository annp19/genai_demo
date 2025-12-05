#ifndef MQTT_H
#define MQTT_H

#include "telemetry.h"

typedef void (*on_data_callback)(TelemetryData data);

void mqtt_init(const char *host, int port, on_data_callback callback);
void mqtt_subscribe(const char *topic);
void mqtt_start_loop();
void mqtt_stop_loop();
void mqtt_cleanup();

#endif // MQTT_H