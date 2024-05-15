#include <stdio.h>
#include <stdlib.h>

#define MAX_SKILLS 4

// Define a struct for a Skill
struct Skill {
    char name[50];
    char description[100];
    enum { DIRECT_ATTACK, TEMPORARY_MODIFIER } type;
    int duration; // Only applicable if type is TEMPORARY_MODIFIER
    int atkModifier;
    int defModifier;
    int hpModifier;
};

// Define a struct for a Character
struct Character {
    char name[50];
    int hp;
    int atk;
    int def;
    struct Skill skills[MAX_SKILLS];
};

// Define a struct for an Enemy
struct Enemy {
    char name[50];
    int hp;
    int atk;
    int def;
};

// Define a struct for a Scenario
struct Option; // Forward declaration
struct Decision {
    char question[100];
    struct Option* options;
    int numOptions;
};

// Define a struct for an Option
struct Option {
    char response[50];
    char narrativeBefore[200];
    struct Enemy enemies[3]; // Assuming up to 3 enemies per option
    char narrativeAfter[200];
};

// Define a struct for a Scenario
struct Scenario {
    char name[50];
    char description[200];
    struct Decision decision;
};

int main() {
    // Example usage
    struct Skill swordMastery = {"Sword Mastery", "Increase attack power with swords", TEMPORARY_MODIFIER, 3, 5, 0, 0};
    struct Skill fireball = {"Fireball", "Launch a fireball at the enemy", DIRECT_ATTACK, 0, 10, 0, 0};

    struct Character player = {"Player", 100, 15, 10, {swordMastery, fireball}};

    struct Enemy goblin = {"Goblin", 50, 8, 5};
    struct Enemy troll = {"Troll", 100, 15, 10};

    struct Option option1 = {"Fight the goblin", "You encounter a goblin in the forest.", {goblin}, "You defeated the goblin and continue your journey."};
    struct Option option2 = {"Fight the troll", "You encounter a troll guarding the bridge.", {troll}, "You vanquished the troll and crossed the bridge."};

    struct Decision decision = {"Which enemy will you fight?", (struct Option[]){option1, option2}, 2};

    struct Scenario forestScenario = {"Forest", "You find yourself in a dense forest.", decision};

    printf("Character: %s\n", player.name);
    printf("Skills:\n");
    for (int i = 0; i < MAX_SKILLS; ++i) {
        printf("- %s: %s\n", player.skills[i].name, player.skills[i].description);
    }

    printf("\nScenario: %s\n", forestScenario.name);
    printf("Description: %s\n", forestScenario.description);
    printf("Decision: %s\n", forestScenario.decision.question);
    for (int i = 0; i < forestScenario.decision.numOptions; ++i) {
        printf("%d. %s\n", i + 1, forestScenario.decision.options[i].response);
    }

    return 0;
}
