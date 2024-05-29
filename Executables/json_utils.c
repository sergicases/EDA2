#include "json_utils.h"
#include "io_utils.h"


int json_get_bool(const cJSON* json, bool default_value)
{
	if (json == NULL)
		return default_value;

	if (cJSON_IsTrue(json))
		return true;

	if (cJSON_IsFalse(json))
		return false;

	return default_value;
}

int json_get_int(const cJSON* json, int default_value)
{
	if (json == NULL || !cJSON_IsNumber(json))
		return default_value;

	return json->valueint;
}

unsigned int json_get_uint(const cJSON* json, unsigned int default_value)
{
	if (json == NULL || !cJSON_IsNumber(json))
		return default_value;

	return (unsigned int)json->valueint;
}
const char* json_get_string(const cJSON* json, const char* default_value)
{
	if (json == NULL || !cJSON_IsString(json))
		return default_value;

	return json->valuestring;
}

bool json_object_get_bool(const cJSON* json, const char* field, bool default_value)
{
	if (json == NULL)
		return default_value;

	return json_get_bool(cJSON_GetObjectItemCaseSensitive(json, field), default_value);
}

int json_object_get_int(const cJSON* json, const char* field, int default_value)
{
	if (json == NULL)
		return default_value;

	return json_get_int(cJSON_GetObjectItemCaseSensitive(json, field), default_value);
}

unsigned int json_object_get_uint(const cJSON* json, const char* field, unsigned int default_value)
{
	if (json == NULL)
		return default_value;

	return json_get_uint(cJSON_GetObjectItemCaseSensitive(json, field), default_value);
}

const char* json_object_get_string(const cJSON* json, const char* field, const char* default_value)
{
	if (json == NULL)
		return default_value;

	return json_get_string(cJSON_GetObjectItemCaseSensitive(json, field), default_value);
}


cJSON* read_json_from_file(const char* filename)
{
	cJSON* json = NULL;
	StringBuffer* data = read_all_text_in_file(filename);
	if (!strbuf_empty(data))
		json = cJSON_ParseWithLength(strbuf_string(data), strbuf_size(data));

	strbuf_delete(data);
	return json;
}
