#ifndef BATTLE_H
#define BATTLE_H

#include "characters.h"

// Enumeration to represent the possible outcomes of a battle
typedef enum {
	BATTLE_RESULT_LOSE, // Player loses the battle
	BATTLE_RESULT_WIN   // Player wins the battle
} BattleResult;

// Structure to represent a battle instance
typedef struct {
	Character* player;   // Pointer to the player's character
	Character* enemy;    // Pointer to the enemy's character
	unsigned int turn;   // Counter to keep track of the number of turns
} Battle;

// Function to run the battle between player and enemy
// Parameters:
// - player: Pointer to the player's character
// - enemy: Pointer to the enemy's character
// Returns:
// - The result of the battle as a BattleResult enum
BattleResult run_battle(Character* player, Character* enemy);

#endif // BATTLE_H
