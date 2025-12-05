#include "data.h"
#include "telemetry.h"
#include "gui.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

TelemetryBuffer chart_buffer;
struct AllData all_data;

void data_init() {
    telemetry_buffer_init(&chart_buffer);
    all_data.data = NULL;
    all_data.count = 0;
    all_data.capacity = 0;
}

void data_add(TelemetryData data) {
    telemetry_buffer_add(&chart_buffer, data);
    if (all_data.count >= all_data.capacity) {
        all_data.capacity = all_data.capacity ? all_data.capacity * 2 : 16;
        all_data.data = realloc(all_data.data, all_data.capacity * sizeof(TelemetryData));
    }
    all_data.data[all_data.count++] = data;
    gui_update_labels(data);
    gui_redraw_charts();
}

TelemetryData data_get_latest() {
    if (chart_buffer.count == 0) return (TelemetryData){0};
    int index = (chart_buffer.head + chart_buffer.count - 1) % CHART_BUFFER_SIZE;
    return chart_buffer.buffer[index];
}

void data_export_csv(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    fprintf(f, "timestamp,speed,battery,latitude,longitude\n");
    for (int i = 0; i < all_data.count; i++) {
        TelemetryData d = all_data.data[i];
        fprintf(f, "%ld,%.2f,%.2f,%.6f,%.6f\n", d.timestamp, d.speed, d.battery, d.latitude, d.longitude);
    }
    fclose(f);
}