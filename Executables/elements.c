#include "elements.h"

// Define string representations for element types
#define ELEMENT_NAME_NORMAL "normal"
#define ELEMENT_NAME_FIRE "fire"
#define ELEMENT_NAME_ICE "ice"
#define ELEMENT_NAME_LIGHTNING "lightning"
#define ELEMENT_NAME_EARTH "earth"
#define ELEMENT_NAME_POISON "poison"
#define ELEMENT_NAME_WATER "water"
#define ELEMENT_NAME_WIND "wind"
#define ELEMENT_NAME_HOLY "holy"

// Convert a JSON object to an Element enum
Element element_from_json(cJSON* json)
{
    // Return normal element if JSON is NULL
    if (json == NULL)
        return ELEMENT_NORMAL;

    // Check if JSON is a number
    if (cJSON_IsNumber(json))
    {
        // Validate and return the element value
        switch (json->valueint)
        {
            case ELEMENT_NORMAL:
            case ELEMENT_FIRE:
            case ELEMENT_ICE:
            case ELEMENT_LIGHTNING:
            case ELEMENT_EARTH:
            case ELEMENT_POISON:
            case ELEMENT_WATER:
            case ELEMENT_WIND:
            case ELEMENT_HOLY:
                return json->valueint;
        }
    }
    // Check if JSON is a string
    else if (cJSON_IsString(json))
    {
        // Compare strings and return the corresponding element
        if (strcmp(json->valuestring, ELEMENT_NAME_NORMAL) == 0) return ELEMENT_NORMAL;
        if (strcmp(json->valuestring, ELEMENT_NAME_FIRE) == 0) return ELEMENT_FIRE;
        if (strcmp(json->valuestring, ELEMENT_NAME_ICE) == 0) return ELEMENT_ICE;
        if (strcmp(json->valuestring, ELEMENT_NAME_LIGHTNING) == 0) return ELEMENT_LIGHTNING;
        if (strcmp(json->valuestring, ELEMENT_NAME_EARTH) == 0) return ELEMENT_EARTH;
        if (strcmp(json->valuestring, ELEMENT_NAME_POISON) == 0) return ELEMENT_POISON;
        if (strcmp(json->valuestring, ELEMENT_NAME_WATER) == 0) return ELEMENT_WATER;
        if (strcmp(json->valuestring, ELEMENT_NAME_WIND) == 0) return ELEMENT_WIND;
        if (strcmp(json->valuestring, ELEMENT_NAME_HOLY) == 0) return ELEMENT_HOLY;
    }

    // Return normal element if JSON does not match any criteria
    return ELEMENT_NORMAL;
}

// Convert an Element enum to a JSON object
cJSON* element_to_json(Element element)
{
    // Return the corresponding string representation of the element
    switch (element)
    {
        case ELEMENT_NORMAL: return cJSON_CreateString(ELEMENT_NAME_NORMAL);
        case ELEMENT_FIRE: return cJSON_CreateString(ELEMENT_NAME_FIRE);
        case ELEMENT_ICE: return cJSON_CreateString(ELEMENT_NAME_ICE);
        case ELEMENT_LIGHTNING: return cJSON_CreateString(ELEMENT_NAME_LIGHTNING);
        case ELEMENT_EARTH: return cJSON_CreateString(ELEMENT_NAME_EARTH);
        case ELEMENT_POISON: return cJSON_CreateString(ELEMENT_NAME_POISON);
        case ELEMENT_WATER: return cJSON_CreateString(ELEMENT_NAME_WATER);
        case ELEMENT_WIND: return cJSON_CreateString(ELEMENT_NAME_WIND);
        case ELEMENT_HOLY: return cJSON_CreateString(ELEMENT_NAME_HOLY);

        // Default case returns the numerical representation of the element
        default:
            return cJSON_CreateNumber((double)element);
    }
}

// Define string representations for element effectiveness types
#define ELEMENT_EFFECTIVENESS_NAME_WEAK "weak"
#define ELEMENT_EFFECTIVENESS_NAME_NORMAL "normal"
#define ELEMENT_EFFECTIVENESS_NAME_RESISTANT "resistant"
#define ELEMENT_EFFECTIVENESS_NAME_IMMUNE "immune"
#define ELEMENT_EFFECTIVENESS_NAME_ABSORB "absorb"

// Convert a JSON object to an ElementEffectiveness enum
ElementEffectiveness element_effectiveness_from_json(const cJSON* json)
{
    // Return normal effectiveness if JSON is NULL
    if (json == NULL)
        return ELEMENT_EFFECTIVENESS_NORMAL;

    // Check if JSON is a number
    if (cJSON_IsNumber(json))
    {
        // Validate and return the effectiveness value
        switch (json->valueint)
        {
            case ELEMENT_EFFECTIVENESS_WEAK:
            case ELEMENT_EFFECTIVENESS_NORMAL:
            case ELEMENT_EFFECTIVENESS_RESISTANT:
            case ELEMENT_EFFECTIVENESS_IMMUNE:
            case ELEMENT_EFFECTIVENESS_ABSORB:
                return json->valueint;
        }
    }
    // Check if JSON is a string
    else if (cJSON_IsString(json))
    {
        // Compare strings and return the corresponding effectiveness
        if (strcmp(json->valuestring, ELEMENT_EFFECTIVENESS_NAME_WEAK) == 0) return ELEMENT_EFFECTIVENESS_WEAK;
        if (strcmp(json->valuestring, ELEMENT_EFFECTIVENESS_NAME_NORMAL) == 0) return ELEMENT_EFFECTIVENESS_NORMAL;
        if (strcmp(json->valuestring, ELEMENT_EFFECTIVENESS_NAME_RESISTANT) == 0) return ELEMENT_EFFECTIVENESS_RESISTANT;
        if (strcmp(json->valuestring, ELEMENT_EFFECTIVENESS_NAME_IMMUNE) == 0) return ELEMENT_EFFECTIVENESS_IMMUNE;
        if (strcmp(json->valuestring, ELEMENT_EFFECTIVENESS_NAME_ABSORB) == 0) return ELEMENT_EFFECTIVENESS_ABSORB;
    }

    // Return normal effectiveness if JSON does not match any criteria
    return ELEMENT_EFFECTIVENESS_NORMAL;
}

// Convert an ElementEffectiveness enum to a JSON object
cJSON* element_effectiveness_to_json(ElementEffectiveness effectiveness)
{
    // Return the corresponding string representation of the effectiveness
    switch (effectiveness)
    {
        case ELEMENT_EFFECTIVENESS_WEAK: return cJSON_CreateString(ELEMENT_EFFECTIVENESS_NAME_WEAK);
        case ELEMENT_EFFECTIVENESS_NORMAL: return cJSON_CreateString(ELEMENT_EFFECTIVENESS_NAME_NORMAL);
        case ELEMENT_EFFECTIVENESS_RESISTANT: return cJSON_CreateString(ELEMENT_EFFECTIVENESS_NAME_RESISTANT);
        case ELEMENT_EFFECTIVENESS_IMMUNE: return cJSON_CreateString(ELEMENT_EFFECTIVENESS_NAME_IMMUNE);
        case ELEMENT_EFFECTIVENESS_ABSORB: return cJSON_CreateString(ELEMENT_EFFECTIVENESS_NAME_ABSORB);

        // Default case returns the numerical representation of the effectiveness
        default:
            return cJSON_CreateNumber((double)effectiveness);
    }
}

// Initialize an Effectiveness structure with default values
void effectiveness_init(Effectiveness* data)
{
    data->normal = ELEMENT_EFFECTIVENESS_NORMAL;
    data->fire = ELEMENT_EFFECTIVENESS_NORMAL;
    data->ice = ELEMENT_EFFECTIVENESS_NORMAL;
    data->lightning = ELEMENT_EFFECTIVENESS_NORMAL;
    data->earth = ELEMENT_EFFECTIVENESS_NORMAL;
    data->poison = ELEMENT_EFFECTIVENESS_NORMAL;
    data->water = ELEMENT_EFFECTIVENESS_NORMAL;
    data->wind = ELEMENT_EFFECTIVENESS_NORMAL;
    data->holy = ELEMENT_EFFECTIVENESS_NORMAL;
}

// Copy the contents of one Effectiveness structure to another
void effectiveness_copy(Effectiveness* dest, const Effectiveness* source)
{
    dest->normal = source->normal;
    dest->fire = source->fire;
    dest->ice = source->ice;
    dest->lightning = source->lightning;
    dest->earth = source->earth;
    dest->poison = source->poison;
    dest->water = source->water;
    dest->wind = source->wind;
    dest->holy = source->holy;
}

// Populate an Effectiveness structure from a JSON object
bool effectiveness_from_json(Effectiveness* effectiveness, const cJSON* json)
{
    // Initialize with default values if JSON is NULL or not an object
    if (json == NULL || !cJSON_IsObject(json))
    {
        effectiveness_init(effectiveness);
        return false;
    }

    // Parse JSON object and populate the Effectiveness structure
    effectiveness->normal = element_effectiveness_from_json(cJSON_GetObjectItemCaseSensitive(json, "normal"));
    effectiveness->fire = element_effectiveness_from_json(cJSON_GetObjectItemCaseSensitive(json, "fire"));
    effectiveness->ice = element_effectiveness_from_json(cJSON_GetObjectItemCaseSensitive(json, "ice"));
    effectiveness->lightning = element_effectiveness_from_json(cJSON_GetObjectItemCaseSensitive(json, "lightning"));
    effectiveness->earth = element_effectiveness_from_json(cJSON_GetObjectItemCaseSensitive(json, "earth"));
    effectiveness->poison = element_effectiveness_from_json(cJSON_GetObjectItemCaseSensitive(json, "poison"));
    effectiveness->water = element_effectiveness_from_json(cJSON_GetObjectItemCaseSensitive(json, "water"));
    effectiveness->wind = element_effectiveness_from_json(cJSON_GetObjectItemCaseSensitive(json, "wind"));
    effectiveness->holy = element_effectiveness_from_json(cJSON_GetObjectItemCaseSensitive(json, "holy"));

    return true;
}

// Convert an Effectiveness structure to a JSON object
cJSON* effectiveness_to_json(const Effectiveness* effectiveness)
{
    cJSON* json = cJSON_CreateObject();
    cJSON_AddItemToObject(json, "normal", element_effectiveness_to_json(effectiveness->normal));
    cJSON_AddItemToObject(json, "fire", element_effectiveness_to_json(effectiveness->fire));
    cJSON_AddItemToObject(json, "ice", element_effectiveness_to_json(effectiveness->ice));
    cJSON_AddItemToObject(json, "lightning", element_effectiveness_to_json(effectiveness->lightning));
    cJSON_AddItemToObject(json, "earth", element_effectiveness_to_json(effectiveness->earth));
    cJSON_AddItemToObject(json, "poison", element_effectiveness_to_json(effectiveness->poison));
    cJSON_AddItemToObject(json, "water", element_effectiveness_to_json(effectiveness->water));
    cJSON_AddItemToObject(json, "wind", element_effectiveness_to_json(effectiveness->wind));
    cJSON_AddItemToObject(json, "holy", element_effectiveness_to_json(effectiveness->holy));
    return json;
}

// Get the effectiveness for a specific element
ElementEffectiveness effectiveness_get_effectiveness_of_normal(const Effectiveness* data) { return data->normal; }
ElementEffectiveness effectiveness_get_effectiveness_of_fire(const Effectiveness* data) { return data->fire; }
ElementEffectiveness effectiveness_get_effectiveness_of_ice(const Effectiveness* data) { return data->ice; }
ElementEffectiveness effectiveness_get_effectiveness_of_lightning(const Effectiveness* data) { return data->lightning; }
ElementEffectiveness effectiveness_get_effectiveness_of_earth(const Effectiveness* data) { return data->earth; }
ElementEffectiveness effectiveness_get_effectiveness_of_poison(const Effectiveness* data) { return data->poison; }
ElementEffectiveness effectiveness_get_effectiveness_of_water(const Effectiveness* data) { return data->water; }
ElementEffectiveness effectiveness_get_effectiveness_of_wind(const Effectiveness* data) { return data->wind; }
ElementEffectiveness effectiveness_get_effectiveness_of_holy(const Effectiveness* data) { return data->holy; }

// Validate the effectiveness value to ensure it is within range
static ElementEffectiveness validate_effectiveness(ElementEffectiveness effectiveness)
{
    if(effectiveness < ELEMENT_EFFECTIVENESS_WEAK) return ELEMENT_EFFECTIVENESS_WEAK;
    if(effectiveness > ELEMENT_EFFECTIVENESS_ABSORB) return ELEMENT_EFFECTIVENESS_ABSORB;
    return effectiveness;
}

// Set the effectiveness for a specific element
void effectiveness_set_effectiveness_of_normal(Effectiveness* data, ElementEffectiveness effectiveness) { data->normal = validate_effectiveness(effectiveness); }
void effectiveness_set_effectiveness_of_fire(Effectiveness* data, ElementEffectiveness effectiveness) { data->fire = validate_effectiveness(effectiveness); }
void effectiveness_set_effectiveness_of_ice(Effectiveness* data, ElementEffectiveness effectiveness) { data->ice = validate_effectiveness(effectiveness); }
void effectiveness_set_effectiveness_of_lightning(Effectiveness* data, ElementEffectiveness effectiveness) { data->lightning = validate_effectiveness(effectiveness); }
void effectiveness_set_effectiveness_of_earth(Effectiveness* data, ElementEffectiveness effectiveness) { data->earth = validate_effectiveness(effectiveness); }
void effectiveness_set_effectiveness_of_poison(Effectiveness* data, ElementEffectiveness effectiveness) { data->poison = validate_effectiveness(effectiveness); }
void effectiveness_set_effectiveness_of_water(Effectiveness* data, ElementEffectiveness effectiveness) { data->water = validate_effectiveness(effectiveness); }
void effectiveness_set_effectiveness_of_wind(Effectiveness* data, ElementEffectiveness effectiveness) { data->wind = validate_effectiveness(effectiveness); }
void effectiveness_set_effectiveness_of_holy(Effectiveness* data, ElementEffectiveness effectiveness) { data->holy = validate_effectiveness(effectiveness); }

// Get the effectiveness of a given element from the Effectiveness structure
ElementEffectiveness effectiveness_get_effectiveness_of(const Effectiveness* data, Element element)
{
    // Return the corresponding effectiveness based on the element type
    switch (element)
    {
        case ELEMENT_NORMAL: return effectiveness_get_effectiveness_of_normal(data);
        case ELEMENT_FIRE: return effectiveness_get_effectiveness_of_fire(data);
        case ELEMENT_ICE: return effectiveness_get_effectiveness_of_ice(data);
        case ELEMENT_LIGHTNING: return effectiveness_get_effectiveness_of_lightning(data);
        case ELEMENT_EARTH: return effectiveness_get_effectiveness_of_earth(data);
        case ELEMENT_POISON: return effectiveness_get_effectiveness_of_poison(data);
        case ELEMENT_WATER: return effectiveness_get_effectiveness_of_water(data);
        case ELEMENT_WIND: return effectiveness_get_effectiveness_of_wind(data);
        case ELEMENT_HOLY: return effectiveness_get_effectiveness_of_holy(data);
    }

    // Print error and return normal effectiveness if element is invalid
    printf("[ERROR]: Invalid Element\n");
    return ELEMENT_EFFECTIVENESS_NORMAL;
}

// Set the effectiveness of a given element in the Effectiveness structure
void effectiveness_set_effectiveness_of(Effectiveness* data, Element element, ElementEffectiveness effectiveness)
{
    // Set the corresponding effectiveness based on the element type
    switch (element)
    {
        case ELEMENT_NORMAL: effectiveness_set_effectiveness_of_normal(data, effectiveness); break;
        case ELEMENT_FIRE: effectiveness_set_effectiveness_of_fire(data, effectiveness); break;
        case ELEMENT_ICE: effectiveness_set_effectiveness_of_lightning(data, effectiveness); break;
        case ELEMENT_LIGHTNING: effectiveness_set_effectiveness_of_normal(data, effectiveness); break;
        case ELEMENT_EARTH: effectiveness_set_effectiveness_of_earth(data, effectiveness); break;
        case ELEMENT_POISON: effectiveness_set_effectiveness_of_poison(data, effectiveness); break;
        case ELEMENT_WATER: effectiveness_set_effectiveness_of_water(data, effectiveness); break;
        case ELEMENT_WIND: effectiveness_set_effectiveness_of_wind(data, effectiveness); break;
        case ELEMENT_HOLY: effectiveness_set_effectiveness_of_holy(data, effectiveness); break;
        default: printf("[ERROR]: Invalid Element\n"); break;
    }
}
