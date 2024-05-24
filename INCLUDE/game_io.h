#ifndef GAME_IO_H
#define GAME_IO_H

#include "abilities.h"
#include "characters.h"
#include "scenario.h"

bool load_game_data_from_file(const char* filename, AbilityPool* ab_pool, CharacterModelPool* ch_pool, Scenario** scenarios_in, size_t* num_scenarios_in);

#endif //GAME_IO_H
