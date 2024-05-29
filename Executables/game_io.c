#include "game_io.h"
#include "json_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for each graph node (scenario)
typedef struct GraphNode {
    char name[50];
    struct GraphNode** neighbors;
    size_t num_neighbors;
} GraphNode;

// Structure for the entire graph
typedef struct {
    GraphNode* nodes;
    size_t num_nodes;
} Graph;

// Function to initialize the graph with predefined scenarios and connections
void initialize_graph(Graph* graph) {
    // Allocates memory for the nodes:
    graph->num_nodes = 4;
    graph->nodes = (GraphNode*)malloc(graph->num_nodes * sizeof(GraphNode));

    // Initializes scenario names
    strcpy(graph->nodes[0].name, "The Forgotten Temple");
    strcpy(graph->nodes[1].name, "The Steam City");
    strcpy(graph->nodes[2].name, "The Quantum Forest");
    strcpy(graph->nodes[3].name, "The Celestial Spire");

    // Initializes neighbors (adjacency list)
    graph->nodes[0].num_neighbors = 2;
    graph->nodes[0].neighbors = (GraphNode**)malloc(2 * sizeof(GraphNode*));
    graph->nodes[0].neighbors[0] = &graph->nodes[1];
    graph->nodes[0].neighbors[1] = &graph->nodes[2];

    graph->nodes[1].num_neighbors = 2;
    graph->nodes[1].neighbors = (GraphNode**)malloc(2 * sizeof(GraphNode*));
    graph->nodes[1].neighbors[0] = &graph->nodes[0];
    graph->nodes[1].neighbors[1] = &graph->nodes[3];

    graph->nodes[2].num_neighbors = 2;
    graph->nodes[2].neighbors = (GraphNode**)malloc(2 * sizeof(GraphNode*));
    graph->nodes[2].neighbors[0] = &graph->nodes[0];
    graph->nodes[2].neighbors[1] = &graph->nodes[3];

    graph->nodes[3].num_neighbors = 2;
    graph->nodes[3].neighbors = (GraphNode**)malloc(2 * sizeof(GraphNode*));
    graph->nodes[3].neighbors[0] = &graph->nodes[1];
    graph->nodes[3].neighbors[1] = &graph->nodes[2];
}

// Function to free the graph memory
void free_graph(Graph* graph) {
    for (size_t i = 0; i < graph->num_nodes; i++) {
        free(graph->nodes[i].neighbors);
    }
    free(graph->nodes);
}

// Static helper function to load abilities from a JSON array into an AbilityPool
static void load_abilities_from_json(AbilityPool* ab_pool, const cJSON* json) {
    // Checks if the JSON array is valid
    if (json == NULL || !cJSON_IsArray(json)) {
        // Prints an error message if the 'abilities' array is not found or invalid
        print_error("'abilities' array not found");
        return;
    }

    // Gets the number of elements in the JSON array
    int len = cJSON_GetArraySize(json);
    for (int i = 0; i < len; i++) {
        // Gets each JSON object in the array
        cJSON* jAb = cJSON_GetArrayItem(json, i);
        // Creates an ability from the JSON object and add it to the AbilityPool
        ability_create_from_json(ab_pool, jAb);
    }
}

// Static helper function to load character models from a JSON array into a CharacterModelPool
static void load_characters_from_json(CharacterModelPool* ch_pool, AbilityPool* ab_pool, const cJSON* json) {
    // Checks if the JSON array is valid
    if (json == NULL || !cJSON_IsArray(json)) {
        // Prints an error message if the 'enemy_models' array is not found or invalid
        print_error("'enemy_models' array not found");
        return;
    }

    // Gets the number of elements in the JSON array
    int len = cJSON_GetArraySize(json);
    for (int i = 0; i < len; i++) {
        // Gets each JSON object in the array
        cJSON* jCh = cJSON_GetArrayItem(json, i);
        // Creates a character model from the JSON object and add it to the CharacterModelPool
        character_model_create_from_json(ch_pool, ab_pool, jCh);
    }
}

// Static helper function to load scenarios from a JSON array into an array of Scenario structures
static void load_scenarios_from_json(Scenario** scenarios_in, size_t* num_scenarios_in, CharacterModelPool* ch_pool, const cJSON* json) {
    // Checks if the JSON array is valid
    if (json == NULL || !cJSON_IsArray(json)) {
        *scenarios_in = NULL;
        *num_scenarios_in = 0;
        // Prints an error message if the 'scenarios' array is not found or invalid
        print_error("'scenarios' array not found");
        return;
    }

    // Gets the number of elements in the JSON array
    int len = cJSON_GetArraySize(json);
    // Allocates memory for the scenarios
    Scenario* scenarios = malloc(sizeof(Scenario) * len);

    for (int i = 0; i < len; i++) {
        // Gets each JSON object in the array
        cJSON* jSc = cJSON_GetArrayItem(json, i);
        // Creates a scenario from the JSON object
        scenario_from_json(&scenarios[i], ch_pool, jSc);
    }

    *scenarios_in = scenarios;
    *num_scenarios_in = (size_t)len;
}

// Function to load game data from a file into AbilityPool, CharacterModelPool, and an array of Scenarios
bool load_game_data_from_file(const char* filename, AbilityPool* ab_pool, CharacterModelPool* ch_pool, Scenario** scenarios_in, size_t* num_scenarios_in) {
    // Reads JSON data from the file
    cJSON* json = read_json_from_file(filename);
    if (json == NULL) return false;

    // Checks if the root JSON object is valid
    if (!cJSON_IsObject(json)) {
        cJSON_Delete(json);
        return false;
    }

    // Initializes the AbilityPool
    ability_pool_init(ab_pool);
    // Gets the 'abilities' JSON array
    cJSON* jAbs = cJSON_GetObjectItemCaseSensitive(json, "abilities");

    // Initializes the CharacterModelPool
    character_model_pool_init(ch_pool);
    // Gets the 'enemy_models' JSON array
    cJSON* jChars = cJSON_GetObjectItemCaseSensitive(json, "enemy_models");
    // Gets the 'scenarios' JSON array
    cJSON* jScs = cJSON_GetObjectItemCaseSensitive(json, "scenarios");

    // Loads abilities from the JSON array
    load_abilities_from_json(ab_pool, jAbs);
    // Loads character models from the JSON array
    load_characters_from_json(ch_pool, ab_pool, jChars);
    // Loads scenarios from the JSON array
    load_scenarios_from_json(scenarios_in, num_scenarios_in, ch_pool, jScs);

    // Creates the scenario graph
    Graph scenario_graph;
    initialize_graph(&scenario_graph);

    // Prints the graph for debugging
    for (size_t i = 0; i < scenario_graph.num_nodes; i++) {
        printf("Scenario: %s\n", scenario_graph.nodes[i].name);
        printf("Neighbors: ");
        for (size_t j = 0; j < scenario_graph.nodes[i].num_neighbors; j++) {
            printf("%s ", scenario_graph.nodes[i].neighbors[j]->name);
        }
        printf("\n");
    }

    // Cleans up the graph
    free_graph(&scenario_graph);

    // Cleans up and free the JSON object
    cJSON_Delete(json);
    return true;
}
