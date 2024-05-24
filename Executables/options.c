#include "options.h"
#include "json_utils.h"

void option_init(
	Option* option,
	CharacterModelPool* characters_pool,
	const char* response_text,
	const char* narrative_before_battle_text,
	const char* narrative_after_battle_text,
	const char* enemies[],
	size_t num_enemies
)
{
	strncpy(option->response_text, response_text, TEXT_LEN);
	strncpy(option->narrative_before_battle_text, narrative_before_battle_text, TEXT_LEN);
	strncpy(option->narrative_after_battle_text, narrative_after_battle_text, TEXT_LEN);

	option->num_enemies = num_enemies;
	if (num_enemies > 0)
	{
		
		option->enemies = malloc(sizeof(Character) * num_enemies);
		for (size_t i = 0; i < option->num_enemies; i++)
		{
			if (!character_model_exists(characters_pool, enemies[i]))
			{
				printf("[ERROR]: Character model '%s' not found.", enemies[i]);
				continue;
			}

			character_init(&option->enemies[i], enemies[i], character_model_get(characters_pool, enemies[i]), false);
		}
	}
	else
	{
		option->enemies = NULL;
	}
}

void option_destroy(Option* option)
{
	if (option->enemies != NULL)
		free(option->enemies);

	option->num_enemies = 0;
	option->enemies = NULL;
}

void option_copy(Option* dest, const Option* source)
{
	strcpy(dest->response_text, source->response_text);
	strcpy(dest->narrative_before_battle_text, source->narrative_before_battle_text);
	strcpy(dest->narrative_after_battle_text, source->narrative_after_battle_text);

	dest->num_enemies = source->num_enemies;
	if (dest->num_enemies > 0)
	{
		dest->enemies = malloc(sizeof(Character) * dest->num_enemies);
		for (size_t i = 0; i < dest->num_enemies; i++)
			character_copy(&dest->enemies[i], &source->enemies[i]);
	}
	else
		dest->enemies = NULL;
}

bool option_from_json(Option* option, CharacterModelPool* characters_pool, const cJSON* json)
{
	if (json == NULL || !cJSON_IsObject(json))
	{
		option_init(option, characters_pool, "", "", "", NULL, 0);
		return false;
	}

	const char* response_text = json_object_get_string(json, "response_text", "");
	const char* narrative_before_battle_text = json_object_get_string(json, "narrative_before_battle_text", "");
	const char* narrative_after_battle_text = json_object_get_string(json, "narrative_after_battle_text", "");
	const char** enemies = NULL;
	int num_enemies = 0;

	cJSON* jEnemies = cJSON_GetObjectItemCaseSensitive(json, "enemies");
	if (jEnemies != NULL && cJSON_GetArraySize(jEnemies) > 0)
	{
		num_enemies = cJSON_GetArraySize(jEnemies);
		enemies = malloc(sizeof(char*) * num_enemies);

		for (int i = 0; i < num_enemies; i++)
		{
			cJSON* jEnemy = cJSON_GetArrayItem(jEnemies, i);
			if (jEnemy == NULL || !cJSON_IsString(jEnemy))
			{
				free(enemies);
				option_init(option, characters_pool, "", "", "", NULL, 0);
				return false;
			}

			enemies[i] = json_get_string(jEnemy, "");
		}
	}

	option_init(option, characters_pool, response_text, narrative_before_battle_text, narrative_after_battle_text, enemies, num_enemies);

	if (enemies != NULL)
		free(enemies);

	return true;
}

cJSON* option_to_json(const Option* option)
{
	cJSON* json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "response_text", option->response_text);
	cJSON_AddStringToObject(json, "narrative_before_battle_text", option->narrative_before_battle_text);
	cJSON_AddStringToObject(json, "narrative_after_battle_text", option->narrative_after_battle_text);

	cJSON* jEnemies = cJSON_CreateArray();
	for (int i = 0; i < option->num_enemies; i++)
	{
		cJSON* jEnemy = cJSON_CreateString(option->enemies[i].model->name);
		cJSON_AddItemToArray(jEnemy, jEnemy);
	}
	cJSON_AddItemToObject(json, "enemies", jEnemies);

	return json;
}

Option* option_new(
	CharacterModelPool* characters_pool,
	const char* response_text,
	const char* narrative_before_battle_text,
	const char* narrative_after_battle_text,
	const char* enemies[],
	size_t num_enemies
)
{
	Option* option = malloc(sizeof(Option));
	option_init(option, characters_pool, response_text, narrative_before_battle_text, narrative_after_battle_text, enemies, num_enemies);
	return option;
}

void option_free(Option* option)
{
	option_destroy(option);
	free(option);
}
