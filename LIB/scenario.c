#include "scenario.h"
#include "json_utils.h"


void scenario_init(
	Scenario* scenario,
	const char* name,
	const char* description,
	Decision* decisions,
	size_t num_decisions
)
{
	if (num_decisions < 1)
		printf("[ERROR]: num_decisions cannot be zero.");

	strncpy(scenario->name, name, NAMES_STR_LEN);
	strncpy(scenario->description, description, TEXT_LEN);

	scenario->num_decisions = num_decisions;
	if (num_decisions > 0)
	{
		scenario->decisions = malloc(sizeof(Decision) * num_decisions);
		for (size_t i = 0; i < num_decisions; i++)
			decision_copy(&scenario->decisions[i], &decisions[i]);
	}
	else
	{
		scenario->decisions = NULL;
	}
}

void scenario_destroy(Scenario* scenario)
{
	if (scenario->decisions != NULL)
	{
		for (size_t i = 0; i < scenario->num_decisions; i++)
			decision_destroy(&scenario->decisions[i]);
		free(scenario->decisions);
	}

	scenario->num_decisions = 0;
	scenario->decisions = NULL;
}

void scenario_copy(Scenario* dest, const Scenario* source)
{
	strcpy(dest->name, source->name);
	strcpy(dest->description, source->description);

	dest->num_decisions = source->num_decisions;
	if (dest->num_decisions > 0)
	{
		dest->decisions = malloc(sizeof(Decision) * dest->num_decisions);
		for (size_t i = 0; i < dest->num_decisions; i++)
			decision_copy(&dest->decisions[i], &source->decisions[i]);
	}
	else
		dest->decisions = NULL;
}

bool scenario_from_json(Scenario* scenario, CharacterModelPool* characters_pool, const cJSON* json)
{
	if (json == NULL || !cJSON_IsObject(json))
	{
		scenario_init(scenario, "", "", NULL, 0);
		return false;
	}

	const char* name = json_object_get_string(json, "name", "");
	const char* description = json_object_get_string(json, "description", "");
	Decision* decisions = NULL;
	int num_decisions = 0;

	cJSON* jDecisions = cJSON_GetObjectItemCaseSensitive(json, "decisions");
	if (jDecisions != NULL && cJSON_GetArraySize(jDecisions) > 0)
	{
		num_decisions = cJSON_GetArraySize(jDecisions);
		decisions = malloc(sizeof(Decision) * num_decisions);

		for (int i = 0; i < num_decisions; i++)
		{
			cJSON* jDecision = cJSON_GetArrayItem(jDecisions, i);
			decision_from_json(&decisions[i], characters_pool, jDecision);
		}
	}

	scenario_init(scenario, name, description, decisions, num_decisions);

	if (decisions != NULL)
		free(decisions);

	return true;
}

cJSON* scenario_to_json(const Scenario* scenario)
{
	cJSON* json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "name", scenario->name);
	cJSON_AddStringToObject(json, "description", scenario->description);

	cJSON* jDecisions = cJSON_CreateArray();
	for (int i = 0; i < scenario->num_decisions; i++)
	{
		cJSON* jDecision = decision_to_json(&scenario->decisions[i]);
		cJSON_AddItemToArray(jDecisions, jDecision);
	}
	cJSON_AddItemToObject(json, "decisions", jDecisions);

	return json;
}

void scenario_init_copy(Scenario* dest, const Scenario* source)
{
	memset(dest, 0, sizeof(Scenario));
	scenario_copy(dest, source);
}

Scenario* scenario_new(
	const char* name,
	const char* description,
	Decision* decisions,
	size_t num_decisions
)
{
	Scenario* scenario = malloc(sizeof(Scenario));
	scenario_init(scenario, name, description, decisions, num_decisions);
	return scenario;
}

void scenario_free(Scenario* scenario)
{
	scenario_destroy(scenario);
	free(scenario);
}