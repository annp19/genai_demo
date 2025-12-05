#ifndef DATA_H
#define DATA_H

#include "telemetry.h"

struct AllData {
    TelemetryData *data;
    int count;
    int capacity;
};

extern TelemetryBuffer chart_buffer;
extern struct AllData all_data;

void data_init();
void data_add(TelemetryData data);
TelemetryData data_get_latest();
void data_export_csv(const char *filename);

#endif // DATA_H