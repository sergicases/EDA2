#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include "commons.h"

int json_get_bool(const cJSON* json, bool default_value);
int json_get_int(const cJSON* json, int default_value);
unsigned int json_get_uint(const cJSON* json, unsigned int default_value);
const char* json_get_string(const cJSON* json, const char* default_value);

bool json_object_get_bool(const cJSON* json, const char* field, bool default_value);
int json_object_get_int(const cJSON* json, const char* field, int default_value);
unsigned int json_object_get_uint(const cJSON* json, const char* field, unsigned int default_value);
const char* json_object_get_string(const cJSON* json, const char* field, const char* default_value);

cJSON* read_json_from_file(const char* filename);

#endif //JSON_UTILS_H
