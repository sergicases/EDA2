// Prevents multiple inclusions of this header file
#ifndef DECISIONS_H
#define DECISIONS_H

// Includes the options header file
#include "options.h"

// Defines a structure to represent a decision
typedef struct {
    char question_text[TEXT_LEN]; // The text of the question for the decision
    Option* options;              // Pointer to an array of options for the decision
    size_t num_options;           // Number of options available for the decision
} Decision;

// Function to initialize a Decision structure
void decision_init(Decision* decision, const char* question_text, Option* options, size_t num_options);

// Function to destroy a Decision structure and free any allocated resources
void decision_destroy(Decision* decision);

// Function to copy the contents of one Decision structure to another
void decision_copy(Decision* dest, const Decision* source);

// Function to populate a Decision structure from a JSON object
bool decision_from_json(Decision* decision, CharacterModelPool* characters_pool, const cJSON* json);

// Function to convert a Decision structure to a JSON object
cJSON* decision_to_json(const Decision* decision);

// Function to create a new Decision structure
Decision* decision_new(const char* question_text, Option* options, size_t num_options);

// Function to free a Decision structure created by decision_new
void decision_free(Decision* decision);

// End of the header guard
#endif // DECISIONS_H
