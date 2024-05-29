// Header guard to prevent multiple inclusions of this header file
#ifndef GAME_IO_H
#define GAME_IO_H
// Including necessary header files which presumably contain definitions for AbilityPool, CharacterModelPool, and Scenario
#include "abilities.h"
#include "characters.h"
#include "scenario.h"
// Function declaration for loading game data from a file:
// Returns a boolean value indicating success (true) or failure (false) of the loading operation.
bool load_game_data_from_file(const char* filename, AbilityPool* ab_pool, CharacterModelPool* ch_pool, Scenario** scenarios_in, size_t* num_scenarios_in);

#endif //GAME_IO_H

