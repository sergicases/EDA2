#ifndef OPTIONS_H
#define OPTIONS_H

#include "commons.h"
#include "characters.h"

typedef struct {
	char response_text[TEXT_LEN];
	char narrative_before_battle_text[TEXT_LEN];
	char narrative_after_battle_text[TEXT_LEN];
	Character* enemies;
	size_t num_enemies;
} Option;

void option_init(
	Option* option,
	CharacterModelPool* characters_pool,
	const char* response_text,
	const char* narrative_before_battle_text,
	const char* narrative_after_battle_text,
	const char* enemies[],
	size_t num_enemies
);

void option_destroy(Option* option);

void option_copy(Option* dest, const Option* source);

bool option_from_json(Option* option, CharacterModelPool* characters_pool, const cJSON* json);
cJSON* option_to_json(const Option* option);

Option* option_new(
	CharacterModelPool* characters_pool,
	const char* response_text,
	const char* narrative_before_battle_text,
	const char* narrative_after_battle_text,
	const char* enemies[],
	size_t num_enemies
);

void option_free(Option* option);

#endif //OPTIONS_H
