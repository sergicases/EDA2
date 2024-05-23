#ifndef CHARACTER_H
#define CHARACTER_H

#include "abilities.h"
#include "elements.h"
#include "stats.h"
#include "menu_utils.h"

#define NUM_ABILITIES_PER_CHARACTER 4                   // Number of abilities each character can have
#define CHARACTER_MODEL_POOL_NODES_TABLE_LEN 16         // Size of the character model pool

// Enumeration to represent the index positions of character abilities
typedef enum {
    CHARACTER_ABILITY_1 = 0,  // First ability
    CHARACTER_ABILITY_2 = 1,  // Second ability
    CHARACTER_ABILITY_3 = 2,  // Third ability
    CHARACTER_ABILITY_4 = 3   // Fourth ability
} CharacterAbility;

// Structure to represent a character model
typedef struct CharacterModel {
    char name[NAMES_STR_LEN];                      // Character model name
    Stats stats;                                   // Character stats
    Effectiveness elements;                        // Elemental effectiveness
    Ability abilities[NUM_ABILITIES_PER_CHARACTER]; // Array of abilities

    struct CharacterModel* next;                   // Pointer to the next character model (for linked list in hash table)
} CharacterModel;

// Structure to represent a pool of character models
typedef struct {
    CharacterModel* table[CHARACTER_MODEL_POOL_NODES_TABLE_LEN]; // Hash table for character models
    size_t size;                                                  // Number of character models in the pool
} CharacterModelPool;

// Function to initialize a character model pool
void character_model_pool_init(CharacterModelPool* pool);

// Function to create a character model and add it to the pool
bool character_model_create(
    CharacterModelPool* pool,
    AbilityPool* abilities_pool,
    const char name[NAMES_STR_LEN],
    const Stats* stats,
    const Effectiveness* elements,
    const char* abilities[NUM_ABILITIES_PER_CHARACTER]
);

// Function to create a character model from JSON and add it to the pool
bool character_model_create_from_json(
    CharacterModelPool* pool,
    AbilityPool* abilities_pool,
    const cJSON* json
);

// Function to check if a character model exists in the pool
bool character_model_exists(CharacterModelPool* pool, const char* model_name);

// Function to get a character model from the pool by name
CharacterModel* character_model_get(CharacterModelPool* pool, const char* model_name);

// Function to initialize a character model
bool character_model_init(
    CharacterModel* model,
    AbilityPool* abilities_pool,
    const char name[NAMES_STR_LEN],
    const Stats* stats,
    const Effectiveness* elements,
    const char* abilities[NUM_ABILITIES_PER_CHARACTER]
);

// Function to copy a character model from source to destination
void character_model_copy(CharacterModel* dest, const CharacterModel* source);

// Function to create a new character model from JSON
CharacterModel* character_model_new_from_json(const cJSON* json, AbilityPool* abilities_pool);

// Function to convert a character model to JSON
cJSON* character_model_to_json(const CharacterModel* model);

// Structure to represent a character instance
typedef struct {
    char name[NAMES_STR_LEN];                // Character name
    CharacterModel* model;                   // Pointer to the character model
    bool is_player_character;                // Flag indicating if the character is controlled by the player

    int current_hp;                          // Current HP of the character
    int current_mp;                          // Current MP of the character
    StatsModifications stats_mods;           // Current stat modifications
    AbilitiesStack abilities_stack;          // Stack of abilities
} Character;

// Function to initialize a character
void character_init(Character* character, char name[NAMES_STR_LEN], CharacterModel* model, bool is_player_character);

// Function to destroy a character and free its resources
void character_destroy(Character* character);

// Function to copy a character from source to destination
void character_copy(Character* dest, const Character* source);

// Function to create a player character using a menu
void menu_create_player_character(Character* player_character, AbilityPool* abilities_pool);

// Function to create a player character from JSON
bool character_player_from_json(Character* player_character, const cJSON* json, AbilityPool* abilities_pool);

// Function to convert a player character model to JSON
cJSON* character_player_model_to_json(const Character* player_character);

#endif // CHARACTER_H
