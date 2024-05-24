// Include the header file for Decision structure and related functions
#include "decisions.h"

// Include utility functions for handling JSON objects
#include "json_utils.h"

// Function to initialize a Decision structure
void decision_init(Decision* decision, const char* question_text, Option* options, size_t num_options)
{
    // Ensure the number of options is at least 1
    if (num_options < 1)
        printf("[ERROR]: Decision.num_options cannot be zero.");

    // Copy the question text to the decision structure
    strncpy(decision->question_text, question_text, TEXT_LEN);
    decision->num_options = num_options;

    // Allocate memory for the options and copy them if there are any
    if (num_options > 0)
    {
        decision->options = malloc(sizeof(Option) * num_options);
        for (size_t i = 0; i < num_options; i++)
            option_copy(&decision->options[i], &options[i]);
    }
    else
    {
        decision->options = NULL;
    }
}

// Function to destroy a Decision structure and free any allocated resources
void decision_destroy(Decision* decision)
{
    // Free each option if they exist
    if (decision->options != NULL)
    {
        for (int i = 0; i < decision->num_options; i++)
            option_destroy(&decision->options[i]);
        free(decision->options);
    }

    // Reset the number of options and the options pointer
    decision->num_options = 0;
    decision->options = NULL;
}

// Function to copy the contents of one Decision structure to another
void decision_copy(Decision* dest, const Decision* source)
{
    // Copy the question text from the source to the destination
    strcpy(dest->question_text, source->question_text);
    dest->num_options = source->num_options;

    // Allocate memory for the options and copy them if there are any
    if (dest->num_options > 0)
    {
        dest->options = malloc(sizeof(Option) * dest->num_options);
        for (size_t i = 0; i < dest->num_options; i++)
            option_copy(&dest->options[i], &source->options[i]);
    }
    else
    {
        dest->options = NULL;
    }
}

// Function to populate a Decision structure from a JSON object
bool decision_from_json(Decision* decision, CharacterModelPool* characters_pool, const cJSON* json)
{
    // Validate the JSON object
    if (json == NULL || !cJSON_IsObject(json))
    {
        decision_init(decision, "", NULL, 0);
        return false;
    }

    // Extract the question text from the JSON object
    const char* question_text = json_object_get_string(json, "question_text", "");
    Option* options = NULL;
    int num_options = 0;

    // Extract the options from the JSON object if they exist
    cJSON* jOptions = cJSON_GetObjectItemCaseSensitive(json, "options");
    if (jOptions != NULL && cJSON_GetArraySize(jOptions) > 0)
    {
        num_options = cJSON_GetArraySize(jOptions);
        options = malloc(sizeof(Option) * num_options);

        // Populate each option from the JSON array
        for (int i = 0; i < num_options; i++)
        {
            cJSON* jOption = cJSON_GetArrayItem(jOptions, i);
            option_from_json(&options[i], characters_pool, jOption);
        }
    }

    // Initialize the Decision structure with the extracted data
    decision_init(decision, question_text, options, num_options);

    // Free the temporary options array if it was allocated
    if (options != NULL)
        free(options);

    return true;
}

// Function to convert a Decision structure to a JSON object
cJSON* decision_to_json(const Decision* decision)
{
    // Create a JSON object to represent the decision
    cJSON* json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "question_text", decision->question_text);

    // Create a JSON array for the options
    cJSON* jOptions = cJSON_CreateArray();
    for (int i = 0; i < decision->num_options; i++)
    {
        // Convert each option to JSON and add it to the array
        cJSON* jOption = option_to_json(&decision->options[i]);
        cJSON_AddItemToArray(jOptions, jOption);
    }
    cJSON_AddItemToObject(json, "options", jOptions);

    return json;
}

// Function to create a new Decision structure
Decision* decision_new(const char* question_text, Option* options, size_t num_options)
{
    // Allocate memory for the new Decision structure
    Decision* decision = malloc(sizeof(Decision));
    // Initialize the Decision structure with the provided data
    decision_init(decision, question_text, options, num_options);
    return decision;
}

// Function to free a Decision structure created by decision_new
void decision_free(Decision* decision)
{
    // Destroy the Decision structure and free the allocated memory
    decision_destroy(decision);
    free(decision);
}
