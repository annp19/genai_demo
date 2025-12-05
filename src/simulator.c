#include <mosquitto.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static pthread_t sim_thread;
static int running = 0;
static struct mosquitto *pub_mosq = NULL;

void *simulator_thread(void *arg) {
    static double lat = 40.7128;
    static double lon = -74.0060;
    static double speed = 0;
    static double battery = 100.0;
    srand(time(NULL));
    while (running) {
        time_t now = time(NULL);
        lat += (rand() % 200 - 100) * 0.0001;
        lon += (rand() % 200 - 100) * 0.0001;
        speed = (rand() % 60);
        battery -= 0.1;
        if (battery < 0) battery = 0;
        char json[256];
        sprintf(json, "{\"timestamp\":%ld,\"speed\":%.2f,\"battery\":%.2f,\"lat\":%.6f,\"lon\":%.6f}", now, speed, battery, lat, lon);
        mosquitto_publish(pub_mosq, NULL, "telemetry", strlen(json), json, 0, false);
        sleep(1);
    }
    return NULL;
}

void simulator_init(const char *host, int port) {
    pub_mosq = mosquitto_new(NULL, true, NULL);
    if (mosquitto_connect(pub_mosq, host, port, 60)) {
        fprintf(stderr, "Simulator unable to connect.\n");
        exit(1);
    }
}

void simulator_start() {
    running = 1;
    pthread_create(&sim_thread, NULL, simulator_thread, NULL);
}

void simulator_stop() {
    running = 0;
    pthread_join(sim_thread, NULL);
}

void simulator_cleanup() {
    mosquitto_destroy(pub_mosq);
}