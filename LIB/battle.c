#include "battle.h"

#define MAX_TURNS 20

typedef enum {
	BATTLE_STATE_RUNNING,
	BATTLE_STATE_PLAYER_WIN,
	BATTLE_STATE_PLAYER_LOSE
} BattleState;

static void battle_ability_effect(Character* attacker, Character* target, const Ability* ability, bool force_critical);

static float battle_get_effectiveness_modifier(Character* character, Element element)
{
	ElementEffectiveness effectiveness = effectiveness_get_effectiveness_of(&character->model->elements, element);
	switch (effectiveness)
	{
		case ELEMENT_EFFECTIVENESS_WEAK: return 2.0f;
		case ELEMENT_EFFECTIVENESS_NORMAL: return 1.0f;
		case ELEMENT_EFFECTIVENESS_RESISTANT: return 0.5f;
		case ELEMENT_EFFECTIVENESS_IMMUNE: return 0.0f;
		case ELEMENT_EFFECTIVENESS_ABSORB: return -1.0f;
		default: return 1.0f;
	}
}

static int battle_heal_character(Character* character, int points)
{
	if (points < 1)
		return 0;

	int old_hp = character->current_hp;
	character->current_hp = min(character->current_hp + points, stats_get_hp(&character->model->stats));
	return character->current_hp - old_hp;
}

static int battle_damage_character(Character* attacker, int target_defense, int points, Element element)
{
	if (points < 1)
		return 0;

	float mod = battle_get_effectiveness_modifier(attacker, element);
	if (mod < 0)
		return -battle_heal_character(attacker, points);

	points = (unsigned int)(points * mod);
	if (points <= 0 || target_defense >= points)
		return 0;

	int old_hp = attacker->current_hp;
	attacker->current_hp = max(attacker->current_hp - (points - target_defense), 0);
	return attacker->current_hp - old_hp;
}

static bool battle_is_magical_ability(const Ability* ability) { return ability->required_mp > 0; }
static int battle_get_character_atk(Character* character, bool magical)
{
	if (magical)
		return stats_get_with_powerups(&character->model->stats, &character->stats_mods, STAT_MAGICAL_ATTACK);
	return stats_get_with_powerups(&character->model->stats, &character->stats_mods, STAT_PHYSICAL_ATTACK);
}
static int battle_get_character_def(Character* character, bool magical)
{
	if (magical)
		return stats_get_with_powerups(&character->model->stats, &character->stats_mods, STAT_MAGICAL_DEFENSE);
	return stats_get_with_powerups(&character->model->stats, &character->stats_mods, STAT_PHYSICAL_DEFENSE);
}

static void battle_ability_effect_normal_damage(Character* attacker, Character* target, const Ability* ability, bool critical_hit)
{
	Element element = ability->effect.element;
	int attacker_atk = battle_get_character_atk(attacker, battle_is_magical_ability(ability));
	int target_def = battle_get_character_def(target, battle_is_magical_ability(ability));
	int power = ability->effect.power;
	int points = attacker_atk + power;
	if (critical_hit) points *= 2;

	int damage = battle_damage_character(attacker, target_def, points, element);
	if (damage > 0)
		printf("�%s causa %d puntos de da�o a %s!\n", attacker->name, points, target->name);
	else if(damage < 0)
		printf("�%s cura %d puntos de salud a %s!\n", attacker->name, -points, target->name);
	else
		printf("�El ataque de %s no afecta a %s!\n", attacker->name, target->name);
}

static void battle_ability_effect_fixed_damage(Character* attacker, Character* target, const Ability* ability, bool critical_hit)
{
	Element element = ability->effect.element;
	int target_def = battle_get_character_def(target, battle_is_magical_ability(ability));
	int points = ability->effect.fixed_value;
	if (critical_hit) points *= 2;

	int damage = battle_damage_character(attacker, target_def, points, element);
	if (damage > 0)
		printf("�%s causa %d puntos de da�o a %s!\n", attacker->name, points, target->name);
	else if (damage < 0)
		printf("�%s cura %d puntos de salud a %s!\n", attacker->name, -points, target->name);
	else
		printf("�El ataque de %s no afecta a %s!\n", attacker->name, target->name);
}

static void battle_ability_effect_normal_heal(Character* attacker, const Ability* ability, bool critical_hit)
{
	int attacker_atk = battle_get_character_atk(attacker, battle_is_magical_ability(ability));
	int power = ability->effect.power;
	int points = attacker_atk + power;
	if (critical_hit) points *= 2;

	int heal = battle_heal_character(attacker, points);
	if (heal > 0)
		printf("�%s se cura %d puntos de salud!\n", attacker->name, points);
	else
		printf("%s no se ha curado nada\n", attacker->name);
}

static void battle_ability_effect_fixed_heal(Character* attacker, const Ability* ability, bool critical_hit)
{
	int points = ability->effect.fixed_value;

	int heal = battle_heal_character(attacker, points);
	if (heal > 0)
		printf("�%s se cura %d puntos de salud!\n", attacker->name, points);
	else
		printf("%s no se ha curado nada\n", attacker->name);
}

static void battle_ability_effect_stat_modification(Character* attacker, Character* target, const Ability* ability, bool critical_hit)
{
	Stat stat = ability->effect.affected_stat;
	int amount = ability->effect.affected_stat_amount;
	Character* character;
	if (ability->effect.affected_stat_to_enemy) character = target;
	else character = attacker;
	if (critical_hit) amount = (int)(amount * 1.25f);

	if (amount == 0)
	{
		printf("No ha ocurrido nada\n");
		return;
	}

	stats_mod_add(&character->stats_mods, stat, amount);

	if(amount > 0)
		printf("%s de %s ha subido %d puntos", stats_name(stat), character->name, amount);
	else
		printf("%s de %s ha bajado %d puntos", stats_name(stat), character->name, -amount);
}

static void battle_ability_effect_custom(Character* attacker, Character* target, const Ability* ability, bool critical_hit)
{
	CustomAbilityEffectId c_id = ability->effect.custom_id;
	if (c_id == CUSTOM_ABILITY_EFFECT_ID_TIME_HIT)
	{
		int k = random_int((int)abilities_stack_size(&attacker->abilities_stack));
		const Ability* past_ability = abilities_stack_get_k(&attacker->abilities_stack, k);
		if (past_ability == NULL)
		{
			printf("No ha ocurrido nada\n");
			return;
		}

		if (past_ability->effect.type == ABILITY_EFFECT_TYPE_CUSTOM && past_ability->effect.custom_id == CUSTOM_ABILITY_EFFECT_ID_TIME_HIT)
		{
			printf("No ha ocurrido nada\n");
			return;
		}

		battle_ability_effect(attacker, target, past_ability, true);
	}
	else
	{
		printf("No ha ocurrido nada\n");
	}
}

static void battle_ability_effect(Character* attacker, Character* target, const Ability* ability, bool force_critical)
{
	bool critical_hit = false;
	if (force_critical || random_int(10000) < 625) // 6.25% //
		critical_hit = true;

	switch (ability->effect.type)
	{
		case ABILITY_EFFECT_TYPE_NORMAL_DAMAGE: battle_ability_effect_normal_damage(attacker, target, ability, critical_hit); break;
		case ABILITY_EFFECT_TYPE_FIXED_DAMAGE: battle_ability_effect_fixed_damage(attacker, target, ability, critical_hit); break;
		case ABILITY_EFFECT_TYPE_NORMAL_HEAL: battle_ability_effect_normal_heal(attacker, ability, critical_hit); break;
		case ABILITY_EFFECT_TYPE_FIXED_HEAL: battle_ability_effect_fixed_heal(attacker, ability, critical_hit); break;
		case ABILITY_EFFECT_TYPE_STAT_MODIFIACTION: battle_ability_effect_stat_modification(attacker, target, ability, critical_hit); break;
		case ABILITY_EFFECT_TYPE_CUSTOM: battle_ability_effect_custom(attacker, target, ability, critical_hit); break;
		default:
			printf("[ERROR]: Unknown ability effect type %d\n", ability->effect.type);
			break;
	}
}

static void battle_print_player_ability(Battle* battle, CharacterAbility ability, int option)
{
	const Ability* ab = &battle->player->model->abilities[ability];
	printf("%d: %s", option, ab->name);
	if (ab->required_mp > 0)
		printf(" [Require %d de m�gia]", ab->required_mp);
	printf("\n");
}

static void battle_init(Battle* battle, Character* player, Character* enemy)
{
	if (player == NULL)
		printf("[ERROR]: battle player character cannot be NULL");
	if (enemy == NULL)
		printf("[ERROR]: battle enemy character cannot be NULL");

	battle->player = player;
	battle->enemy = player;
	battle->turn = 0;
}

static void battle_init_character(Character* character)
{
	character->current_hp = stats_get_hp(&character->model->stats);
	character->current_mp = stats_get_mp(&character->model->stats);
	stats_mod_init(&character->stats_mods);
	abilities_stack_init(&character->abilities_stack);
}

static void battle_clear_character(Character* character)
{
	character->current_hp = 0;
	character->current_mp = 0;
	stats_mod_init(&character->stats_mods);
	abilities_stack_clear(&character->abilities_stack);
}

static bool battle_is_player_alive(Battle* battle) { return battle->player->current_hp > 0; }
static bool battle_is_enemy_alive(Battle* battle) { return battle->enemy->current_hp > 0; }

static BattleState battle_state(Battle* battle)
{
	if (battle->turn >= MAX_TURNS) return BATTLE_STATE_PLAYER_LOSE;
	if (!battle_is_player_alive(battle)) return BATTLE_STATE_PLAYER_LOSE;
	if (!battle_is_enemy_alive(battle)) return BATTLE_STATE_PLAYER_WIN;
	return BATTLE_STATE_RUNNING;
}

static bool battle_can_use_ability(Character* character, CharacterAbility ability)
{
	const Ability* ab = &character->model->abilities[ability];
	if ((int)ab->required_mp > character->current_mp)
		return false;

	return true;
}

static void battle_use_ability(Character* attacker, Character* target, CharacterAbility ability)
{
	const Ability* ab = &attacker->model->abilities[ability];
	if (battle_is_magical_ability(ab))
		attacker->current_mp = max(attacker->current_mp - ab->required_mp, 0);

	battle_ability_effect(attacker, target, ab, false);
	abilities_stack_push(&attacker->abilities_stack, ab);
}

static CharacterAbility battle_select_player_ability(Battle* battle)
{
	Character* player = battle->player;

	int option = 0;
	while (option < 1 && option > 4)
	{
		printf("�Qu� movimiento quieres usar?\n");
		for (int i = 0; i < NUM_ABILITIES_PER_CHARACTER; i++)
			battle_print_player_ability(battle, i, i + 1);

		option = read_option();

		if (option < 1 && option > 4)
			printf("Opci�n no v�lida\n");
		else if (!battle_can_use_ability(player, option - 1))
		{
			printf("No tienes suficiente m�gia para usar este movimiento\n");
			option = 0;
		}
	}

	return option - 1;
}

static CharacterAbility battle_select_enemy_ability(Battle* battle)
{
	CharacterAbility available[NUM_ABILITIES_PER_CHARACTER];
	int count = 0;

	for (int i = 0; i < NUM_ABILITIES_PER_CHARACTER; i++)
	{
		if (battle_can_use_ability(battle->enemy, i))
			available[count++] = i;
	}

	if (count == 0)
		return random_int(NUM_ABILITIES_PER_CHARACTER);

	return available[random_int(count)];
}

static void battle_do_turn(Battle* battle, CharacterAbility player_ability, CharacterAbility enemy_ability)
{
	if (battle_is_player_alive(battle))
		battle_use_ability(battle->player, battle->enemy, player_ability);

	if (battle_is_enemy_alive(battle))
		battle_use_ability(battle->enemy, battle->player, enemy_ability);

	battle->turn++;
}

static void battle_print_characters_state(Battle* battle)
{
	int player_cur_hp = battle->player->current_hp;
	int player_cur_mp = battle->player->current_mp;
	int player_hp = stats_get(&battle->player->model->stats, STAT_HEALTH_POINTS);
	int player_mp = stats_get(&battle->player->model->stats, STAT_MAGICAL_POINTS);

	int enemy_cur_hp = battle->enemy->current_hp;
	int enemy_hp = stats_get(&battle->enemy->model->stats, STAT_HEALTH_POINTS);
	int enemy_hp_per = (int)(enemy_cur_hp / ((float)enemy_hp) * 100.0f);

	printf("[ENEMY]\n%s\nSalud: %d%%\n\n", battle->enemy->name, enemy_hp_per);
	printf("[PLAYER]\n%s\nnSalud: %d/%d\nMagia: %d/%d\n\n", battle->player->name, player_cur_hp, player_hp, player_cur_mp, player_mp);
}

static void battle_loop(Battle* battle)
{
	while (battle_state(battle) == BATTLE_STATE_RUNNING)
	{
		battle_print_characters_state(battle);
		CharacterAbility player_ability = battle_select_player_ability(battle);
		CharacterAbility enemy_ability = battle_select_enemy_ability(battle);

		battle_do_turn(battle, player_ability, enemy_ability);
	}

	if (battle_state(battle) == BATTLE_STATE_PLAYER_WIN)
		printf("�Enemigo %s ha sido derrotado!\n�%s gana la batalla!\n", battle->enemy->name, battle->player->name);
	else
		printf("�Enemigo %s ha vencido a %s!\n�%s pierde la batalla!\n", battle->enemy->name, battle->player->name, battle->player->name);
}

BattleResult run_battle(Character* player, Character* enemy)
{
	printf("�%s quiere luchar!\n\n", enemy->name);

	Battle battle;
	battle_init(&battle, player, enemy);
	battle_init_character(player);
	battle_init_character(enemy);

	battle_loop(&battle);

	BattleState state = battle_state(&battle);
	battle_clear_character(player);
	battle_clear_character(enemy);

	if (state == BATTLE_STATE_PLAYER_WIN)
		return BATTLE_RESULT_WIN;
	return BATTLE_RESULT_LOSE;
}