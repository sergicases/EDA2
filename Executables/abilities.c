#include "abilities.h"
#include "json_utils.h"

#define ABILITY_EFFECT_TYPE_NAME_NORMAL_DAMAGE "normal_damage"
#define ABILITY_EFFECT_TYPE_NAME_FIXED_DAMAGE "fixed_damage"
#define ABILITY_EFFECT_TYPE_NAME_NORMAL_HEAL "normal_heal"
#define ABILITY_EFFECT_TYPE_NAME_FIXED_HEAL "fixed_heal"
#define ABILITY_EFFECT_TYPE_NAME_STAT_MODIFIACTION "stat_mod"
#define ABILITY_EFFECT_TYPE_NAME_CUSTOM "custom"



void ability_type_init_as_normal_damage(AbilityEffect* effect, Element element, unsigned int power)
{
	effect->type = ABILITY_EFFECT_TYPE_NORMAL_DAMAGE;
	effect->element = element;
	effect->power = power;
	effect->fixed_value = 0;
	effect->affected_stat = 0;
	effect->affected_stat_amount = 0;
	effect->affected_stat_to_enemy = false;
	effect->custom_id = 0;
}

void ability_type_init_as_fixed_damage(AbilityEffect* effect, Element element, unsigned int fixed_value)
{
	effect->type = ABILITY_EFFECT_TYPE_FIXED_DAMAGE;
	effect->element = element;
	effect->power = 0;
	effect->fixed_value = fixed_value;
	effect->affected_stat = 0;
	effect->affected_stat_amount = 0;
	effect->affected_stat_to_enemy = false;
	effect->custom_id = 0;
}

void ability_type_init_as_normal_heal(AbilityEffect* effect, unsigned int power)
{
	effect->type = ABILITY_EFFECT_TYPE_NORMAL_HEAL;
	effect->element = 0;
	effect->power = power;
	effect->fixed_value = 0;
	effect->affected_stat = 0;
	effect->affected_stat_amount = 0;
	effect->affected_stat_to_enemy = false;
	effect->custom_id = 0;
}

void ability_type_init_as_fixed_heal(AbilityEffect* effect, unsigned int fixed_value)
{
	effect->type = ABILITY_EFFECT_TYPE_FIXED_HEAL;
	effect->element = 0;
	effect->power = 0;
	effect->fixed_value = fixed_value;
	effect->affected_stat = 0;
	effect->affected_stat_amount = 0;
	effect->affected_stat_to_enemy = false;
	effect->custom_id = 0;
}

void ability_type_init_as_stat_mod(AbilityEffect* effect, Stat stat, int modification, bool to_enemy)
{
	effect->type = ABILITY_EFFECT_TYPE_STAT_MODIFIACTION;
	effect->element = 0;
	effect->power = 0;
	effect->fixed_value = 0;
	effect->affected_stat = stat;
	effect->affected_stat_amount = modification;
	effect->affected_stat_to_enemy = to_enemy;
	effect->custom_id = 0;
}

void ability_type_init_as_custom(AbilityEffect* effect, CustomAbilityEffectId id)
{
	effect->type = ABILITY_EFFECT_TYPE_CUSTOM;
	effect->element = 0;
	effect->power = 0;
	effect->fixed_value = 0;
	effect->affected_stat = 0;
	effect->affected_stat_amount = 0;
	effect->affected_stat_to_enemy = false;
	effect->custom_id = id;
}

void ability_type_copy(AbilityEffect* dest, const AbilityEffect* source)
{
	dest->type = source->type;
	dest->element = source->element;
	dest->power = source->power;
	dest->fixed_value = source->fixed_value;
	dest->affected_stat = source->affected_stat;
	dest->affected_stat_amount = source->affected_stat_amount;
	dest->affected_stat_to_enemy = source->affected_stat_to_enemy;
	dest->custom_id = source->custom_id;
}


static AbilityEffectType ability_type_json_to_type(const cJSON* jType)
{
	if (cJSON_IsNumber(jType))
		return jType->valueint;
	else if (cJSON_IsString(jType))
	{
		if (strcmp(jType->valuestring, ABILITY_EFFECT_TYPE_NAME_NORMAL_DAMAGE) == 0)
			return ABILITY_EFFECT_TYPE_NORMAL_DAMAGE;
		if (strcmp(jType->valuestring, ABILITY_EFFECT_TYPE_NAME_FIXED_DAMAGE) == 0)
			return ABILITY_EFFECT_TYPE_FIXED_DAMAGE;
		if (strcmp(jType->valuestring, ABILITY_EFFECT_TYPE_NAME_NORMAL_HEAL) == 0)
			return ABILITY_EFFECT_TYPE_NORMAL_HEAL;
		if (strcmp(jType->valuestring, ABILITY_EFFECT_TYPE_NAME_FIXED_HEAL) == 0)
			return ABILITY_EFFECT_TYPE_FIXED_HEAL;
		if (strcmp(jType->valuestring, ABILITY_EFFECT_TYPE_NAME_STAT_MODIFIACTION) == 0)
			return ABILITY_EFFECT_TYPE_STAT_MODIFIACTION;
		if (strcmp(jType->valuestring, ABILITY_EFFECT_TYPE_NAME_CUSTOM) == 0)
			return ABILITY_EFFECT_TYPE_CUSTOM;

		return -1;
	}
	return -1;
}
static cJSON* ability_type_type_to_json(AbilityEffectType type)
{
	switch (type)
	{
		case ABILITY_EFFECT_TYPE_NORMAL_DAMAGE: return cJSON_CreateString(ABILITY_EFFECT_TYPE_NAME_NORMAL_DAMAGE);
		case ABILITY_EFFECT_TYPE_FIXED_DAMAGE: return cJSON_CreateString(ABILITY_EFFECT_TYPE_NAME_FIXED_DAMAGE);
		case ABILITY_EFFECT_TYPE_NORMAL_HEAL: return cJSON_CreateString(ABILITY_EFFECT_TYPE_NAME_NORMAL_HEAL);
		case ABILITY_EFFECT_TYPE_FIXED_HEAL: return cJSON_CreateString(ABILITY_EFFECT_TYPE_NAME_FIXED_HEAL);
		case ABILITY_EFFECT_TYPE_STAT_MODIFIACTION: return cJSON_CreateString(ABILITY_EFFECT_TYPE_NAME_STAT_MODIFIACTION);
		case ABILITY_EFFECT_TYPE_CUSTOM: return cJSON_CreateString(ABILITY_EFFECT_TYPE_NAME_CUSTOM);

		default:
			return cJSON_CreateNumber((double)type);
	}
}

bool ability_type_from_json(AbilityEffect* effect, const cJSON* json)
{
	if (!json || !cJSON_IsObject(json))
	{
		memset(effect, 0, sizeof(AbilityEffect));
		print_error("Expected valid json on ability_type_load_from_json");
		return false;
	}

	cJSON* jType = cJSON_GetObjectItemCaseSensitive(json, "type");
	if (jType == NULL)
	{
		memset(effect, 0, sizeof(AbilityEffect));
		print_error("'type' field on ability_type not found");
		return false;
	}
	if (!cJSON_IsNumber(jType) && !cJSON_IsString(jType))
	{
		memset(effect, 0, sizeof(AbilityEffect));
		print_error("Expected valid integer or string on 'type' field on ability_type");
		return false;
	}

	AbilityEffectType type = ability_type_json_to_type(jType);
	if (type == ABILITY_EFFECT_TYPE_NORMAL_DAMAGE)
	{
		Element element = element_from_json(cJSON_GetObjectItemCaseSensitive(json, "element"));
		unsigned int power = json_object_get_uint(json, "power", 0);
		ability_type_init_as_normal_damage(effect, element, power);
	}
	else if (type == ABILITY_EFFECT_TYPE_FIXED_DAMAGE)
	{
		Element element = element_from_json(cJSON_GetObjectItemCaseSensitive(json, "element"));
		unsigned int fixed_value = json_object_get_uint(json, "fixed_value", 0);
		ability_type_init_as_fixed_damage(effect, element, fixed_value);
	}
	else if (type == ABILITY_EFFECT_TYPE_NORMAL_HEAL)
	{
		unsigned int power = json_object_get_uint(json, "power", 0);
		ability_type_init_as_normal_heal(effect, power);
	}
	else if (type == ABILITY_EFFECT_TYPE_FIXED_HEAL)
	{
		unsigned int fixed_value = json_object_get_uint(json, "fixed_value", 0);
		ability_type_init_as_fixed_heal(effect, fixed_value);
	}
	else if (type == ABILITY_EFFECT_TYPE_STAT_MODIFIACTION)
	{
		Stat stat = stat_from_json(cJSON_GetObjectItemCaseSensitive(json, "stat"));
		int modification = json_object_get_int(json, "modification", 0);
		bool to_enemy = json_object_get_bool(json, "to_enemy", false);
		ability_type_init_as_stat_mod(effect, stat, modification, to_enemy);
	}
	else if (type == ABILITY_EFFECT_TYPE_CUSTOM)
	{
		CustomAbilityEffectId custom_id = json_object_get_int(json, "custom_id", 0);
		ability_type_init_as_custom(effect, custom_id);
	}
	else
	{
		print_error("Expected valid integer or string on 'type' field on ability_type");
		return false;
	}

	return true;
}

cJSON* ability_type_to_json(const AbilityEffect* effect)
{
	cJSON* json = cJSON_CreateObject();
	cJSON_AddItemToObject(json, "type", ability_type_type_to_json(effect->type));
	switch (effect->type)
	{
		case ABILITY_EFFECT_TYPE_NORMAL_DAMAGE:
			cJSON_AddItemToObject(json, "element", element_to_json(effect->element));
			cJSON_AddNumberToObject(json, "power", effect->power);
			break;

		case ABILITY_EFFECT_TYPE_FIXED_DAMAGE:
			cJSON_AddItemToObject(json, "element", element_to_json(effect->element));
			cJSON_AddNumberToObject(json, "fixed_value", effect->fixed_value);
			break;

		case ABILITY_EFFECT_TYPE_NORMAL_HEAL:
			cJSON_AddNumberToObject(json, "power", effect->power);
			break;

		case ABILITY_EFFECT_TYPE_FIXED_HEAL:
			cJSON_AddNumberToObject(json, "fixed_value", effect->fixed_value);
			break;

		case ABILITY_EFFECT_TYPE_STAT_MODIFIACTION:
			cJSON_AddItemToObject(json, "stat", stat_to_json(effect->affected_stat));
			cJSON_AddNumberToObject(json, "modification", effect->affected_stat_amount);
			cJSON_AddBoolToObject(json, "to_enemy", effect->affected_stat_to_enemy);
			break;

		case ABILITY_EFFECT_TYPE_CUSTOM:
			cJSON_AddNumberToObject(json, "custom_id", effect->custom_id);
			break;
	}

	return json;
}






void ability_init_as_normal_damage(Ability* ability, const char* name, bool single_use, unsigned int required_mp, Element element, unsigned int power)
{
	strcpy(ability->name, name);
	ability_type_init_as_normal_damage(&ability->effect, element, power);
	ability->single_use = single_use;
    ability->required_mp = required_mp;
}

void ability_init_as_fixed_damage(Ability* ability, const char* name, bool single_use, unsigned int required_mp, Element element, unsigned int fixed_value)
{
	strcpy(ability->name, name);
	ability_type_init_as_fixed_damage(&ability->effect, element, fixed_value);
	ability->single_use = single_use;
    ability->required_mp = required_mp;
}

void ability_init_as_normal_heal(Ability* ability, const char* name, bool single_use, unsigned int required_mp, unsigned int power)
{
	strcpy(ability->name, name);
	ability_type_init_as_normal_heal(&ability->effect, power);
	ability->single_use = single_use;
    ability->required_mp = required_mp;
}

void ability_init_as_fixed_heal(Ability* ability, const char* name, bool single_use, unsigned int required_mp, unsigned int fixed_value)
{
	strcpy(ability->name, name);
	ability_type_init_as_fixed_heal(&ability->effect, fixed_value);
	ability->single_use = single_use;
    ability->required_mp = required_mp;
}

void ability_init_as_stat_mod(Ability* ability, const char* name, bool single_use, unsigned int required_mp, Stat stat, int modification, bool to_enemy)
{
	strcpy(ability->name, name);
	ability_type_init_as_stat_mod(&ability->effect, stat, modification, to_enemy);
	ability->single_use = single_use;
    ability->required_mp = required_mp;
}

void ability_init_as_custom(Ability* ability, const char* name, bool single_use, unsigned int required_mp, CustomAbilityEffectId id)
{
	strcpy(ability->name, name);
	ability_type_init_as_custom(&ability->effect, id);
	ability->single_use = single_use;
    ability->required_mp = required_mp;
}

void ability_copy(Ability* dest, const Ability* source)
{
	strcpy(dest->name, source->name);
	ability_type_copy(&dest->effect, &source->effect);
	dest->single_use = source->single_use;
	dest->required_mp = source->required_mp;
}

bool ability_from_json(Ability* ability, const cJSON* json)
{
	if (json == NULL || !cJSON_IsObject(json))
	{
		memset(ability, 0, sizeof(Ability));
		print_error("Expected valid json on ability_from_json");
		return false;
	}

	const char* name = json_object_get_string(json, "name", "");
	strncpy(ability->name, name, NAMES_STR_LEN);

	ability->single_use = json_object_get_bool(json, "single_use", false);
	ability->required_mp = json_object_get_uint(json, "mp", 0);

	if (!ability_type_from_json(&ability->effect, cJSON_GetObjectItemCaseSensitive(json, "effect")))
		return false;

	return true;
}

cJSON* ability_to_json(const Ability* ability)
{
	cJSON* json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "name", ability->name);
	cJSON_AddBoolToObject(json, "single_use", ability->single_use);
	cJSON_AddNumberToObject(json, "mp", ability->required_mp);
	cJSON_AddItemToObject(json, "mp", ability_type_to_json(&ability->effect));
	return json;
}




void abilities_stack_init(AbilitiesStack* stack)
{
	stack->top = NULL;
	stack->size = 0;
}

void abilities_stack_push(AbilitiesStack* stack, const Ability* ability)
{
	AbilitiesStackNode* node = malloc(sizeof(AbilitiesStackNode));
	node->ability = ability;

	if (stack->top == NULL)
	{
		stack->top = node;
		node->next = NULL;
	}
	else
	{
		node->next = stack->top;
		stack->top = node;
	}
	stack->size++;
}

const Ability* abilities_stack_pop(AbilitiesStack* stack)
{
	if (stack->top == NULL)
		return NULL;

	AbilitiesStackNode* node = stack->top;
	stack->top = node->next;

	const Ability* ability = node->ability;
	free(node);
	stack->size--;

	return ability;
}

const Ability* abilities_stack_top(const AbilitiesStack* stack) { return stack->top == NULL ? NULL : stack->top->ability; }

bool abilities_stack_empty(const AbilitiesStack* stack) { return stack->top == NULL; }

size_t abilities_stack_size(const AbilitiesStack* stack) { return stack->size; }

void abilities_stack_clear(AbilitiesStack* stack)
{
	AbilitiesStackNode* node = stack->top;
	while (node != NULL)
	{
		AbilitiesStackNode* next = node->next;
		free(node);
		node = next;
	}
	stack->top = NULL;
	stack->size = 0;
}

const Ability* abilities_stack_get_k(AbilitiesStack* stack, int index)
{
	if (abilities_stack_empty(stack))
		return NULL;

	AbilitiesStack temp;
	abilities_stack_init(&temp);

	const Ability* ability = NULL;
	while (index > 0 && !abilities_stack_empty(stack))
	{
		ability = abilities_stack_pop(stack);
		index--;
		abilities_stack_push(&temp, ability);
	}

	while (!abilities_stack_empty(&temp))
		abilities_stack_push(stack, abilities_stack_pop(&temp));

	return ability;
}

void abilities_stack_copy(AbilitiesStack* dest, const AbilitiesStack* const_source)
{
	abilities_stack_init(dest);
	if (abilities_stack_empty(const_source))
		return;

	AbilitiesStack temp;
	abilities_stack_init(&temp);

	AbilitiesStack* source = (AbilitiesStack*)const_source;
	while (!abilities_stack_empty(source))
	{
		const Ability* ab = abilities_stack_pop(source);
		abilities_stack_push(&temp, ab);
	}

	while (!abilities_stack_empty(&temp))
	{
		const Ability* ab = abilities_stack_pop(&temp);
		abilities_stack_push(source, ab);
		abilities_stack_push(dest, ab);
	}
}



void ability_pool_init(AbilityPool* pool)
{
	for (int i = 0; i < ABILITIES_POOL_NODES_TABLE_LEN; i++)
		pool->table[i] = NULL;
	pool->size = 0;
}

static size_t ability_pool_hash(const char* ab_name)
{
	return string_hash(ABILITIES_POOL_NODES_TABLE_LEN, ab_name);
}

static bool ability_pool_insert_node(AbilityPool* pool, const char* ab_name, AbilityPoolNode* node)
{
	if (ability_exists(pool, ab_name))
	{
		free(node);
		printf("[ERROR]: Ability '%s' already exists", ab_name);
		return false;
	}

	size_t hash = ability_pool_hash(ab_name);
	if (pool->table[hash] == NULL)
	{
		pool->table[hash] = node;
		node->next = NULL;
	}
	else
	{
		node->next = pool->table[hash];
		pool->table[hash] = node;
	}
	pool->size++;

	return true;
}

bool ability_create_normal_damage(AbilityPool* pool, const char* name, bool single_use, unsigned int required_mp, Element element, unsigned int power)
{
	AbilityPoolNode* node = malloc(sizeof(AbilityPoolNode));
	ability_init_as_normal_damage(&node->ability, name, single_use, required_mp, element, power);
	return ability_pool_insert_node(pool, name, node);
}

bool ability_create_fixed_damage(AbilityPool* pool, const char* name, bool single_use, unsigned int required_mp, Element element, unsigned int fixed_value)
{
	AbilityPoolNode* node = malloc(sizeof(AbilityPoolNode));
	ability_init_as_fixed_damage(&node->ability, name, single_use, required_mp, element, fixed_value);
	return ability_pool_insert_node(pool, name, node);
}

bool ability_create_normal_heal(AbilityPool* pool, const char* name, bool single_use, unsigned int required_mp, unsigned int power)
{
	AbilityPoolNode* node = malloc(sizeof(AbilityPoolNode));
	ability_init_as_normal_heal(&node->ability, name, single_use, required_mp, power);
	return ability_pool_insert_node(pool, name, node);
}

bool ability_create_fixed_heal(AbilityPool* pool, const char* name, bool single_use, unsigned int required_mp, unsigned int fixed_value)
{
	AbilityPoolNode* node = malloc(sizeof(AbilityPoolNode));
	ability_init_as_fixed_heal(&node->ability, name, single_use, required_mp, fixed_value);
	return ability_pool_insert_node(pool, name, node);
}

bool ability_create_stat_mod(AbilityPool* pool, const char* name, bool single_use, unsigned int required_mp, Stat stat, int modification, bool to_enemy)
{
	AbilityPoolNode* node = malloc(sizeof(AbilityPoolNode));
	ability_init_as_stat_mod(&node->ability, name, single_use, required_mp, stat, modification, to_enemy);
	return ability_pool_insert_node(pool, name, node);
}

bool ability_create_custom(AbilityPool* pool, const char* name, bool single_use, unsigned int required_mp, CustomAbilityEffectId id)
{
	AbilityPoolNode* node = malloc(sizeof(AbilityPoolNode));
	ability_init_as_custom(&node->ability, name, single_use, required_mp, id);
	return ability_pool_insert_node(pool, name, node);
}

bool ability_create_from_json(AbilityPool* pool, const cJSON* json)
{
	AbilityPoolNode* node = malloc(sizeof(AbilityPoolNode));
	if (!ability_from_json(&node->ability, json))
	{
		free(node);
		return false;
	}

	return ability_pool_insert_node(pool, node->ability.name, node);
}

bool ability_exists(AbilityPool* pool, const char* ab_name)
{
	return ability_get(pool, ab_name) != NULL;
}

Ability* ability_get(AbilityPool* pool, const char* ab_name)
{
	size_t hash = ability_pool_hash(ab_name);
	for (AbilityPoolNode* node = pool->table[hash]; node != NULL; node = node->next)
	{
		if (strcmp(node->ability.name, ab_name) == 0)
			return &node->ability;
	}
	return NULL;
}

void ability_pool_print(AbilityPool* pool)
{
	for (int i = 0; i < ABILITIES_POOL_NODES_TABLE_LEN; i++)
	{
		for (AbilityPoolNode* node = pool->table[i]; node != NULL; node = node->next)
		{
			printf("- %s\n", node->ability.name);
		}
	}
}
