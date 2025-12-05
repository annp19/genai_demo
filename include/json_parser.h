#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "telemetry.h"

TelemetryData parse_telemetry_json(const char *json_str);

#endif // JSON_PARSER_H