#include <cjson/cJSON.h>
#include "json_parser.h"
#include <string.h>

TelemetryData parse_telemetry_json(const char *json_str) {
    TelemetryData data = {0};
    cJSON *root = cJSON_Parse(json_str);
    if (!root) return data;

    cJSON *ts = cJSON_GetObjectItem(root, "timestamp");
    if (ts && cJSON_IsNumber(ts)) data.timestamp = (time_t)ts->valuedouble;

    cJSON *spd = cJSON_GetObjectItem(root, "speed");
    if (spd && cJSON_IsNumber(spd)) data.speed = spd->valuedouble;

    cJSON *bat = cJSON_GetObjectItem(root, "battery");
    if (bat && cJSON_IsNumber(bat)) data.battery = bat->valuedouble;

    cJSON *lat = cJSON_GetObjectItem(root, "lat");
    if (lat && cJSON_IsNumber(lat)) data.latitude = lat->valuedouble;

    cJSON *lon = cJSON_GetObjectItem(root, "lon");
    if (lon && cJSON_IsNumber(lon)) data.longitude = lon->valuedouble;

    cJSON_Delete(root);
    return data;
}