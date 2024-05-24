#include "scenario.h"
#include "characters.h"
#include "menu_utils.h"
#include "game_io.h"
#include <stdio.h>
#define MAX_SCENARIOS 4

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static void menu_scenario(Scenario* scenario, Character* player_character)
{

}

static void menu_main(Scenario* scenarios, size_t num_scenarios, AbilityPool* abilities_pool)
{
	while (true)
	{
		printf("Main menu\n\n");
		printf("1 - New game\n");
		printf("2 - Exit\n");

		int option = read_option();
		if (option == 1)
		{
			Character player_character;

			printf("Antes de comenzar la historia, vamos a construir tu personaje.\n\n");
			menu_create_player_character(&player_character, abilities_pool);

			printf("\nAhora podemos comenzar la partida!\n\n");
			menu_scenario(&scenarios[0], &player_character);

			character_destroy(&player_character);
		}

		if (option == 2) break;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main()
{
	AbilityPool abilities_pool;
	CharacterModelPool enemies_pool;
	Scenario* scenarios;
	size_t num_scenarios;

	if (!load_game_data_from_file("gamedata.json", &abilities_pool, &enemies_pool, &scenarios, &num_scenarios))
		return 1;

	menu_main(scenarios, num_scenarios, &abilities_pool);
}
