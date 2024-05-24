// Prevent multiple inclusions of this header file
#ifndef ELEMENTS_H
#define ELEMENTS_H

// Include the commons header file
#include "commons.h"

// Enumeration for different elements
typedef enum {
    ELEMENT_NORMAL,
    ELEMENT_FIRE,
    ELEMENT_ICE,
    ELEMENT_LIGHTNING,
    ELEMENT_EARTH,
    ELEMENT_POISON,
    ELEMENT_WATER,
    ELEMENT_WIND,
    ELEMENT_HOLY
} Element;

// Function to convert a JSON object to an Element
Element element_from_json(cJSON* json);

// Function to convert an Element to a JSON object
cJSON* element_to_json(Element element);

// Enumeration for different levels of element effectiveness
typedef enum {
    ELEMENT_EFFECTIVENESS_WEAK,         // damage * 2
    ELEMENT_EFFECTIVENESS_NORMAL,       // damage
    ELEMENT_EFFECTIVENESS_RESISTANT,    // damage / 2
    ELEMENT_EFFECTIVENESS_IMMUNE,       // 0 damage
    ELEMENT_EFFECTIVENESS_ABSORB        // heal
} ElementEffectiveness;

// Function to convert a JSON object to an ElementEffectiveness
ElementEffectiveness element_effectiveness_from_json(const cJSON* json);

// Function to convert an ElementEffectiveness to a JSON object
cJSON* element_effectiveness_to_json(ElementEffectiveness effectiveness);

// Structure to represent the effectiveness of various elements
typedef struct {
    ElementEffectiveness normal;
    ElementEffectiveness fire;
    ElementEffectiveness ice;
    ElementEffectiveness lightning;
    ElementEffectiveness earth;
    ElementEffectiveness poison;
    ElementEffectiveness water;
    ElementEffectiveness wind;
    ElementEffectiveness holy;
} Effectiveness;

// Function to initialize an Effectiveness structure
void effectiveness_init(Effectiveness* data);

// Function to copy the contents of one Effectiveness structure to another
void effectiveness_copy(Effectiveness* dest, const Effectiveness* source);

// Function to populate an Effectiveness structure from a JSON object
bool effectiveness_from_json(Effectiveness* effectiveness, const cJSON* json);

// Function to convert an Effectiveness structure to a JSON object
cJSON* effectiveness_to_json(const Effectiveness* effectiveness);

// Functions to get the effectiveness of various elements from an Effectiveness structure
ElementEffectiveness effectiveness_get_effectiveness_of_normal(const Effectiveness* data);
ElementEffectiveness effectiveness_get_effectiveness_of_fire(const Effectiveness* data);
ElementEffectiveness effectiveness_get_effectiveness_of_ice(const Effectiveness* data);
ElementEffectiveness effectiveness_get_effectiveness_of_lightning(const Effectiveness* data);
ElementEffectiveness effectiveness_get_effectiveness_of_earth(const Effectiveness* data);
ElementEffectiveness effectiveness_get_effectiveness_of_poison(const Effectiveness* data);
ElementEffectiveness effectiveness_get_effectiveness_of_water(const Effectiveness* data);
ElementEffectiveness effectiveness_get_effectiveness_of_wind(const Effectiveness* data);
ElementEffectiveness effectiveness_get_effectiveness_of_holy(const Effectiveness* data);

// Functions to set the effectiveness of various elements in an Effectiveness structure
void effectiveness_set_effectiveness_of_normal(Effectiveness* data, ElementEffectiveness effectiveness);
void effectiveness_set_effectiveness_of_fire(Effectiveness* data, ElementEffectiveness effectiveness);
void effectiveness_set_effectiveness_of_ice(Effectiveness* data, ElementEffectiveness effectiveness);
void effectiveness_set_effectiveness_of_lightning(Effectiveness* data, ElementEffectiveness effectiveness);
void effectiveness_set_effectiveness_of_earth(Effectiveness* data, ElementEffectiveness effectiveness);
void effectiveness_set_effectiveness_of_poison(Effectiveness* data, ElementEffectiveness effectiveness);
void effectiveness_set_effectiveness_of_water(Effectiveness* data, ElementEffectiveness effectiveness);
void effectiveness_set_effectiveness_of_wind(Effectiveness* data, ElementEffectiveness effectiveness);
void effectiveness_set_effectiveness_of_holy(Effectiveness* data, ElementEffectiveness effectiveness);

// Function to get the effectiveness of a specific element from an Effectiveness structure
ElementEffectiveness effectiveness_get_effectiveness_of(const Effectiveness* data, Element element);

// Function to set the effectiveness of a specific element in an Effectiveness structure
void effectiveness_set_effectiveness_of(Effectiveness* data, Element element, ElementEffectiveness effectiveness);

// End of the header guard
#endif // ELEMENTS_H
