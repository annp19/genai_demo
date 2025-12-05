#include "telemetry.h"

void telemetry_buffer_init(TelemetryBuffer *buf) {
    buf->head = 0;
    buf->count = 0;
}

void telemetry_buffer_add(TelemetryBuffer *buf, TelemetryData data) {
    buf->buffer[buf->head] = data;
    buf->head = (buf->head + 1) % CHART_BUFFER_SIZE;
    if (buf->count < CHART_BUFFER_SIZE) buf->count++;
}

TelemetryData *telemetry_buffer_get(TelemetryBuffer *buf, int index) {
    if (index >= buf->count) return NULL;
    int pos = (buf->head - buf->count + index) % CHART_BUFFER_SIZE;
    if (pos < 0) pos += CHART_BUFFER_SIZE;
    return &buf->buffer[pos];
}