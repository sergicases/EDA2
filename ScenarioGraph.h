#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define a structure for the graph node representing a scenario
struct Scenario {
    char description[100];
    struct Scenario** adjacents;
    int numAdjacents;
};

// Function to create a new scenario node
struct Scenario* createScenario(const char* description) {
    struct Scenario* scenario = (struct Scenario*)malloc(sizeof(struct Scenario));
    if (scenario != NULL) {
        snprintf(scenario->description, sizeof(scenario->description), "%s", description);
        scenario->numAdjacents = 0;
        scenario->adjacents = NULL;
    }
    return scenario;
}

// Function to add an adjacent scenario (edge) to a scenario node
void addAdjacent(struct Scenario* source, struct Scenario* adjacent) {
    source->numAdjacents++;
    source->adjacents = (struct Scenario**)realloc(source->adjacents, source->numAdjacents * sizeof(struct Scenario*));
    source->adjacents[source->numAdjacents - 1] = adjacent;
}

// Function to navigate to an adjacent scenario
struct Scenario* navigate(struct Scenario* currentScenario) {
    int choice;
    printf("\nChoose your next scenario:\n");
    for (int i = 0; i < currentScenario->numAdjacents; ++i) {
        printf("%d. %s\n", i + 1, currentScenario->adjacents[i]->description);
    }
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= currentScenario->numAdjacents) {
        return currentScenario->adjacents[choice - 1];
    } else {
        printf("Invalid choice. Returning to current scenario.\n");
        return currentScenario;
    }
}

// Example usage
int main() {
    // Create scenarios
    struct Scenario* scenario1 = createScenario("Forest");
    struct Scenario* scenario2 = createScenario("Cave");
    struct Scenario* scenario3 = createScenario("Castle");

    // Connect scenarios (add edges)
    addAdjacent(scenario1, scenario2);
    addAdjacent(scenario1, scenario3);
    addAdjacent(scenario2, scenario1);
    addAdjacent(scenario3, scenario1);

    // Start the game at scenario1
    struct Scenario* currentScenario = scenario1;

    // Game loop
    while (true) {
        printf("\nCurrent Scenario: %s\n", currentScenario->description);
        // Perform actions specific to each scenario

        // Navigate to an adjacent scenario
        currentScenario = navigate(currentScenario);
    }

    // Free memory (not implemented in this example)

    return 0;
}
