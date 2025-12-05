#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <time.h>

#define CHART_BUFFER_SIZE 100

typedef struct {
    time_t timestamp;
    double speed; // km/h
    double battery; // percentage 0-100
    double latitude;
    double longitude;
} TelemetryData;

typedef struct {
    TelemetryData buffer[CHART_BUFFER_SIZE];
    int head;
    int count;
} TelemetryBuffer;

void telemetry_buffer_init(TelemetryBuffer *buf);
void telemetry_buffer_add(TelemetryBuffer *buf, TelemetryData data);
TelemetryData *telemetry_buffer_get(TelemetryBuffer *buf, int index);

#endif // TELEMETRY_H