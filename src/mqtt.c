#include <mosquitto.h>
#include "mqtt.h"
#include "json_parser.h"
#include <stdio.h>
#include <stdlib.h>

static struct mosquitto *mosq = NULL;
static on_data_callback cb = NULL;

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
    if (message->payloadlen) {
        TelemetryData data = parse_telemetry_json((const char *)message->payload);
        if (cb) cb(data);
    }
}

void mqtt_init(const char *host, int port, on_data_callback callback) {
    cb = callback;
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        fprintf(stderr, "Error: Out of memory.\n");
        exit(1);
    }
    mosquitto_message_callback_set(mosq, on_message);
    if (mosquitto_connect(mosq, host, port, 60)) {
        fprintf(stderr, "Unable to connect.\n");
        exit(1);
    }
}

void mqtt_subscribe(const char *topic) {
    mosquitto_subscribe(mosq, NULL, topic, 1);
}

void mqtt_start_loop() {
    mosquitto_loop_start(mosq);
}

void mqtt_stop_loop() {
    mosquitto_loop_stop(mosq, true);
}

void mqtt_cleanup() {
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}