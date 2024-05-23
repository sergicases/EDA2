#ifndef SCENARIO_H
#define SCENARIO_H

#include "decisions.h"

typedef struct {
	char name[NAMES_STR_LEN];
	char description[TEXT_LEN];
	Decision* decisions;
	size_t num_decisions;
} Scenario;

void scenario_init(
	Scenario* scenario,
	const char* name,
	const char* description,
	Decision* decisions,
	size_t num_decisions
);

void scenario_destroy(Scenario* scenario);

void scenario_copy(Scenario* dest, const Scenario* source);

bool scenario_from_json(Scenario* scenario, CharacterModelPool* characters_pool, const cJSON* json);
cJSON* scenario_to_json(const Scenario* scenario);

Scenario* scenario_new(
	const char* name,
	const char* description,
	Decision* decisions,
	size_t num_decisions
);

void scenario_free(Scenario* scenario);

#endif //SCENARIO_H