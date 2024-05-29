#include "characters.h"
#include "menu_utils.h"
#include "game_io.h"
#include <stdio.h>
#define MAX_SCENARIOS 4

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Function to handle scenario-specific menu
static void menu_scenario(Scenario* scenario, Character* player_character)
{}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Function to handle the main menu
static void menu_main(Scenario* scenarios, size_t num_scenarios, AbilityPool* abilities_pool)
{
	while (true)
	{
		printf("Main menu\n\n"); // Print main menu options
		printf("1 - New game\n");
		printf("2 - Exit\n");

		int option = read_option(); // Read user's option
		if (option == 1)
		{
			Character player_character;

			printf("Let's choose the main character.\n\n"); // Prompt to create a new character
			menu_create_player_character(&player_character, abilities_pool); // Create player character

			printf("\nWe can now start the game!\n\n"); // Start the game with the first scenario
			menu_scenario(&scenarios[0], &player_character);

			character_destroy(&player_character); // Destroy the player character after use
		}

		if (option == 2) break; // Exit the main menu loop
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Main function
int main()
{
	AbilityPool abilities_pool; // Initialize abilities pool
	CharacterModelPool enemies_pool; // Initialize enemies pool
	Scenario* scenarios; // Pointer to scenarios array
	size_t num_scenarios; // Number of scenarios

	// Load game data from file
	if (!load_game_data_from_file("gamedata.json", &abilities_pool, &enemies_pool, &scenarios, &num_scenarios))
		return 1; // Exit if loading game data fails

	menu_main(scenarios, num_scenarios, &abilities_pool); // Start the main menu
}
