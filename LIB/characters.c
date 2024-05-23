#include "characters.h"
#include "json_utils.h"

static size_t character_model_pool_hash(const char* model_name)
{
    return string_hash(CHARACTER_MODEL_POOL_NODES_TABLE_LEN, model_name);
}



void character_model_pool_init(CharacterModelPool* pool)
{
    for (int i = 0; i < CHARACTER_MODEL_POOL_NODES_TABLE_LEN; i++)
        pool->table[i] = NULL;
    pool->size = 0;
}

static void character_model_add_to_pool(CharacterModelPool* pool, CharacterModel* model)
{
    size_t hash = character_model_pool_hash(model->name);
    if (pool->table[hash] == NULL)
    {
        pool->table[hash] = model;
        model->next = NULL;
    }
    else
    {
        model->next = pool->table[hash];
        pool->table[hash] = model;
    }
    pool->size++;
}

bool character_model_create(
    CharacterModelPool* pool,
    AbilityPool* abilities_pool,
    const char name[NAMES_STR_LEN],
    const Stats* stats,
    const Effectiveness* elements,
    const char* abilities[NUM_ABILITIES_PER_CHARACTER]
)
{
    if (character_model_exists(pool, name))
    {
        printf("[ERROR]: Character model '%s' already exists", name);
        return false;
    }
    CharacterModel* model = malloc(sizeof(CharacterModel));
    if (!character_model_init(model, abilities_pool, name, stats, elements, abilities))
    {
        free(model);
        return false;
    }

    character_model_add_to_pool(pool, model);
    return true;
}

bool character_model_create_from_json(
    CharacterModelPool* pool,
    AbilityPool* abilities_pool,
    const cJSON* json
)
{
    CharacterModel* model = character_model_new_from_json(json, abilities_pool);
    if (model == NULL)
        return false;

    if (character_model_exists(pool, model->name))
    {
        printf("[ERROR]: Character model '%s' already exists", model->name);
        free(model);
        return false;
    }

    character_model_add_to_pool(pool, model);
    return true;
}

bool character_model_exists(CharacterModelPool* pool, const char* model_name)
{
    return character_model_get(pool, model_name) != NULL;
}

CharacterModel* character_model_get(CharacterModelPool* pool, const char* model_name)
{
    size_t hash = character_model_pool_hash(model_name);
    for (CharacterModel* model = pool->table[hash]; model != NULL; model = model->next)
    {
        if (strcmp(model->name, model_name) == 0)
            return model;
    }
    return NULL;
}

bool character_model_init(
    CharacterModel* model,
    AbilityPool* abilities_pool,
    const char name[NAMES_STR_LEN],
    const Stats* stats,
    const Effectiveness* elements,
    const char* abilities[NUM_ABILITIES_PER_CHARACTER]
)
{
    strcpy(model->name, name);
    stats_copy(&model->stats, stats);
    effectiveness_copy(&model->elements, elements);
    for (int i = 0; i < NUM_ABILITIES_PER_CHARACTER; i++)
    {
        if (!ability_exists(abilities_pool, abilities[i]))
        {
            printf("[ERROR]: Ability '%s' not found", abilities[i]);
            return false;
        }
        Ability* ability = ability_get(abilities_pool, abilities[i]);
        ability_copy(&model->abilities[i], ability);
    }
    return true;
}

void character_model_copy(CharacterModel* dest, const CharacterModel* source)
{
    strcpy(dest->name, source->name);
    stats_copy(&dest->stats, &source->stats);
    effectiveness_copy(&dest->elements, &source->elements);
    for (int i = 0; i < NUM_ABILITIES_PER_CHARACTER; i++)
        ability_copy(&dest->abilities[i], &source->abilities[i]);
}

CharacterModel* character_model_new_from_json(const cJSON* json, AbilityPool* abilities_pool)
{
    if (json == NULL || !cJSON_IsObject(json))
    {
        print_error("Expected valid json object on CharacterModel creation");
        return NULL;
    }
    const char* jname = json_object_get_string(json, "name", "");
    if (jname == NULL || strlen(jname) < 1)
    {
        print_error("CharacterModel name cannot be NULL or empty");
        return NULL;
    }

    char name[NAMES_STR_LEN];
    Stats stats;
    Effectiveness elements;
    const char* abilities[NUM_ABILITIES_PER_CHARACTER] = { NULL, NULL, NULL, NULL };


    strncpy(name, jname, NAMES_STR_LEN);
    stats_from_json(&stats, cJSON_GetObjectItemCaseSensitive(json, "stats"));
    effectiveness_from_json(&elements, cJSON_GetObjectItemCaseSensitive(json, "elements"));
    
    cJSON* jAbArray = cJSON_GetObjectItemCaseSensitive(json, "abilities");
    if (jAbArray == NULL || !cJSON_IsArray(jAbArray))
    {
        print_error("Expected valid array of 4 strings in CharacterModel abilities field.");
        return NULL;
    }

    int len = cJSON_GetArraySize(jAbArray);
    if(len < 4)
    {
        print_error("Expected valid array of 4 strings in CharacterModel abilities field.");
        return NULL;
    }
    for (int i = 0; i < 4; i++)
    {
        cJSON* jAbName = cJSON_GetArrayItem(jAbArray, i);
        const char* ab_name = json_get_string(jAbName, "");
        if (ab_name == NULL || strlen(ab_name) < 1)
        {
            print_error("CharacterModel abilities name cannot be null or empty.");
            return NULL;
        }

        abilities[i] = ab_name;
    }

    CharacterModel* model = malloc(sizeof(CharacterModel));
    if (model == NULL)
        return NULL;

    if (!character_model_init(model, abilities_pool, name, &stats, &elements, abilities))
    {
        free(model);
        return false;
    }

    return model;
}

cJSON* character_model_to_json(const CharacterModel* model)
{
    cJSON* json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", model->name);
    cJSON_AddItemToObject(json, "stats", stats_to_json(&model->stats));
    cJSON_AddItemToObject(json, "elements", effectiveness_to_json(&model->elements));

    cJSON* jAbilities = cJSON_CreateArray();
    for (int i = 0; i < NUM_ABILITIES_PER_CHARACTER; i++)
        cJSON_AddItemToArray(jAbilities, cJSON_CreateString(model->abilities[i].name));
    cJSON_AddItemToObject(json, "abilities", jAbilities);

    return json;
}




void character_init(Character* character, char name[NAMES_STR_LEN], CharacterModel* model, bool is_player_character)
{
    strcpy(character->name, name);
    character->model = model;
    character->is_player_character = is_player_character;

    character->current_hp = 0;
    character->current_mp = 0;
    stats_mod_init(&character->stats_mods);
    abilities_stack_init(&character->abilities_stack);
}

void character_destroy(Character* character)
{
    if(character->is_player_character && character->model != NULL)
        free(character->model);

    character->model = NULL;
    character->is_player_character = false;
}

void character_copy(Character* dest, const Character* source)
{
    strcpy(dest->name, source->name);
    dest->is_player_character = source->is_player_character;
    if (dest->is_player_character)
    {
        dest->model = malloc(sizeof(CharacterModel));
        character_model_copy(dest->model, source->model);
    }
    else
        dest->model = source->model;

    dest->current_hp = source->current_hp;
    dest->current_mp = source->current_mp;
    stats_mod_copy(&dest->stats_mods, &source->stats_mods);
    abilities_stack_copy(&dest->abilities_stack, &source->abilities_stack);
}


static void read_ability_name_input(AbilityPool* abilities_pool, char abilities[NUM_ABILITIES_PER_CHARACTER][NAMES_STR_LEN], size_t ability_idx)
{
    char message[150];
    sprintf(message, "Escribe el nombre de la habilidad n�mero %d para tu personaje (debe ser una de la lista anterior)", (int)(ability_idx + 1));

    while (true)
    {
        read_string_input(message, abilities[ability_idx], NAMES_STR_LEN);
        if (ability_exists(abilities_pool, abilities[ability_idx]))
            break;

        printf("El nombre de habilidad que has escrito no existe. Prueba de nuevo\n");
    }
}

void menu_create_player_character(Character* player_character, AbilityPool* abilities_pool)
{
    char name[NAMES_STR_LEN];
    Stats stats;
    Effectiveness elements;
    char abilities[NUM_ABILITIES_PER_CHARACTER][NAMES_STR_LEN];

    read_string_input("Escribe el nombre de tu personaje (m�ximo 64 caracteres):", name, NAMES_STR_LEN);

    stats_init(&stats);
    stats_set_hp(&stats, read_int_input("Escribe cuantos puntos de salud tendr� tu personaje:"));
    stats_set_mp(&stats, read_int_input("Escribe cuantos puntos de magia tendr� tu personaje:"));
    stats_set_atk(&stats, read_int_input("Escribe el valor de Ataque F�sico que tendr� tu personaje:"));
    stats_set_def(&stats, read_int_input("Escribe el valor de Defensa F�sica que tendr� tu personaje:"));
    stats_set_m_atk(&stats, read_int_input("Escribe el valor de Ataque M�gico que tendr� tu personaje:"));
    stats_set_m_def(&stats, read_int_input("Escribe el valor de Defensa M�gica que tendr� tu personaje:"));

    effectiveness_init(&elements);

    ability_pool_print(abilities_pool);

    for (int i = 0; i < NUM_ABILITIES_PER_CHARACTER; i++)
        read_ability_name_input(abilities_pool, abilities, i);

    const char* abilities_names[NUM_ABILITIES_PER_CHARACTER] = { abilities[0], abilities[1], abilities[2], abilities[3] };
    CharacterModel* model = malloc(sizeof(CharacterModel));
    character_model_init(model, abilities_pool, name, &stats, &elements, abilities_names);

    character_init(player_character, name, model, true);

    printf("Personaje creado con �xito!\n");
}



bool character_player_from_json(Character* player_character, const cJSON* json, AbilityPool* abilities_pool)
{
    CharacterModel* model = character_model_new_from_json(json, abilities_pool);
    if (model == NULL)
        return false;

    character_init(player_character, model->name, model, true);
    return true;
}

cJSON* character_player_model_to_json(const Character* player_character)
{
    return character_model_to_json(player_character->model);
}