#ifndef ABILITIES_H
#define ABILITIES_H

#include "elements.h"
#include "stats.h"

#define ABILITIES_POOL_NODES_TABLE_LEN 16

typedef enum {
    ABILITY_EFFECT_TYPE_NORMAL_DAMAGE,
    ABILITY_EFFECT_TYPE_FIXED_DAMAGE,
    ABILITY_EFFECT_TYPE_NORMAL_HEAL,
    ABILITY_EFFECT_TYPE_FIXED_HEAL,
    ABILITY_EFFECT_TYPE_STAT_MODIFIACTION,
    ABILITY_EFFECT_TYPE_CUSTOM,
} AbilityEffectType;

typedef enum {
    CUSTOM_ABILITY_EFFECT_ID_TIME_HIT
} CustomAbilityEffectId;

typedef struct {
    AbilityEffectType type;
    Element element;                    // -> NormalDamage, FixedDamage
    unsigned int power;                 // -> NormalDamage, NoramlHeal
    unsigned int fixed_value;           // -> FixedDamage, FixedHeal
    Stat affected_stat;                 // -> StatModification
    int affected_stat_amount;           // -> StatModification
    bool affected_stat_to_enemy;        // -> StatModification
    CustomAbilityEffectId custom_id;    // -> Custom
} AbilityEffect;

void ability_type_init_as_normal_damage(AbilityEffect* effect, Element element, unsigned int power);
void ability_type_init_as_fixed_damage(AbilityEffect* effect, Element element, unsigned int fixed_value);
void ability_type_init_as_normal_heal(AbilityEffect* effect, unsigned int power);
void ability_type_init_as_fixed_heal(AbilityEffect* effect, unsigned int fixed_value);
void ability_type_init_as_stat_mod(AbilityEffect* effect, Stat stat, int modification, bool to_enemy);
void ability_type_init_as_custom(AbilityEffect* effect, CustomAbilityEffectId id);

void ability_type_copy(AbilityEffect* dest, const AbilityEffect* source);

bool ability_type_from_json(AbilityEffect* effect, const cJSON* json);
cJSON* ability_type_to_json(const AbilityEffect* effect);



typedef struct {
    char name[NAMES_STR_LEN];
    AbilityEffect effect;
    bool single_use;
    unsigned int required_mp;
} Ability;

void ability_init_as_normal_damage(Ability* ability, const char* name, bool single_use, unsigned int required_mp, Element element, unsigned int power);
void ability_init_as_fixed_damage(Ability* ability, const char* name, bool single_use, unsigned int required_mp, Element element, unsigned int fixed_value);
void ability_init_as_normal_heal(Ability* ability, const char* name, bool single_use, unsigned int required_mp, unsigned int power);
void ability_init_as_fixed_heal(Ability* ability, const char* name, bool single_use, unsigned int required_mp, unsigned int fixed_value);
void ability_init_as_stat_mod(Ability* ability, const char* name, bool single_use, unsigned int required_mp, Stat stat, int modification, bool to_enemy);
void ability_init_as_custom(Ability* ability, const char* name, bool single_use, unsigned int required_mp, CustomAbilityEffectId id);

void ability_copy(Ability* dest, const Ability* source);

bool ability_from_json(Ability* ability, const cJSON* json);
cJSON* ability_to_json(const Ability* ability);



typedef struct AbilitiesStackNode {
    const Ability* ability;
    struct AbilitiesStackNode* next;
} AbilitiesStackNode;
typedef struct {
    AbilitiesStackNode* top;
    size_t size;
} AbilitiesStack;

void abilities_stack_init(AbilitiesStack* stack);
void abilities_stack_push(AbilitiesStack* stack, const Ability* ability);
const Ability* abilities_stack_pop(AbilitiesStack* stack);
const Ability* abilities_stack_top(const AbilitiesStack* stack);
bool abilities_stack_empty(const AbilitiesStack* stack);
size_t abilities_stack_size(const AbilitiesStack* stack);
void abilities_stack_clear(AbilitiesStack* stack);

const Ability* abilities_stack_get_k(AbilitiesStack* stack, int index);
void abilities_stack_copy(AbilitiesStack* dest, const AbilitiesStack* source);



typedef struct AbilityPoolNode {
    Ability ability;
    struct AbilityPoolNode* next;
} AbilityPoolNode;
typedef struct {
    AbilityPoolNode* table[ABILITIES_POOL_NODES_TABLE_LEN];
    size_t size;
} AbilityPool;

void ability_pool_init(AbilityPool* pool);

bool ability_create_normal_damage(AbilityPool* pool, const char* name, bool single_use, unsigned int required_mp, Element element, unsigned int power);
bool ability_create_fixed_damage(AbilityPool* pool, const char* name, bool single_use, unsigned int required_mp, Element element, unsigned int fixed_value);
bool ability_create_normal_heal(AbilityPool* pool, const char* name, bool single_use, unsigned int required_mp, unsigned int power);
bool ability_create_fixed_heal(AbilityPool* pool, const char* name, bool single_use, unsigned int required_mp, unsigned int fixed_value);
bool ability_create_stat_mod(AbilityPool* pool, const char* name, bool single_use, unsigned int required_mp, Stat stat, int modification, bool to_enemy);
bool ability_create_custom(AbilityPool* pool, const char* name, bool single_use, unsigned int required_mp, CustomAbilityEffectId id);

bool ability_create_from_json(AbilityPool* pool, const cJSON* json);

bool ability_exists(AbilityPool* pool, const char* ab_name);
Ability* ability_get(AbilityPool* pool, const char* ab_name);

void ability_pool_print(AbilityPool* pool);

#endif //ABILITIES_H