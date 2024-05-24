#include "game_io.h"
#include "json_utils.h"

static void load_abilities_from_json(AbilityPool* ab_pool, const cJSON* json)
{
	if (json == NULL || !cJSON_IsArray(json))
	{
		print_error("'abilities' array not found");
		return;
	}

	int len = cJSON_GetArraySize(json);
	for (int i = 0; i < len; i++)
	{
		cJSON* jAb = cJSON_GetArrayItem(json, i);
		ability_create_from_json(ab_pool, jAb);
	}
}

static void load_characters_from_json(CharacterModelPool* ch_pool, AbilityPool* ab_pool, const cJSON* json)
{
	if (json == NULL || !cJSON_IsArray(json))
	{
		print_error("'enemy_models' array not found");
		return;
	}

	int len = cJSON_GetArraySize(json);
	for (int i = 0; i < len; i++)
	{
		cJSON* jCh = cJSON_GetArrayItem(json, i);
		character_model_create_from_json(ch_pool, ab_pool, jCh);
	}
}

static void load_scenarios_from_json(Scenario** scenarios_in, size_t* num_scenarios_in, CharacterModelPool* ch_pool, const cJSON* json)
{
	if (json == NULL || !cJSON_IsArray(json))
	{
		*scenarios_in = NULL;
		*num_scenarios_in = 0;
		print_error("'scenarios' array not found");
		return;
	}

	int len = cJSON_GetArraySize(json);
	Scenario* scenarios = malloc(sizeof(Scenario) * len);

	for (int i = 0; i < len; i++)
	{
		cJSON* jSc = cJSON_GetArrayItem(json, i);
		scenario_from_json(&scenarios[i], ch_pool, jSc);
	}

	*scenarios_in = scenarios;
	*num_scenarios_in = (size_t)len;
}

bool load_game_data_from_file(const char* filename, AbilityPool* ab_pool, CharacterModelPool* ch_pool, Scenario** scenarios_in, size_t* num_scenarios_in)
{
	cJSON* json = read_json_from_file(filename);
	if (json == NULL)
		return false;

	if (!cJSON_IsObject(json))
	{
		cJSON_Delete(json);
		return false;
	}

	ability_pool_init(ab_pool);
	cJSON* jAbs = cJSON_GetObjectItemCaseSensitive(json, "abilities");

	character_model_pool_init(ch_pool);
	cJSON* jChars = cJSON_GetObjectItemCaseSensitive(json, "enemy_models");
	cJSON* jScs = cJSON_GetObjectItemCaseSensitive(json, "scenarios");

	load_abilities_from_json(ab_pool, jAbs);
	load_characters_from_json(ch_pool, ab_pool, jChars);
	load_scenarios_from_json(scenarios_in, num_scenarios_in, ch_pool, jScs);

	cJSON_Delete(json);
	return true;
}
