#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_TYPE_LEN 20
#define MAX_POKEMON 1024

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char type[MAX_TYPE_LEN];
    int evolves_to; // 0 if no evolution
} Pokemon;

int loadPokedex(const char* filename, Pokemon pokedex[], int max) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening CSV file");
        return 0;
    }

    char line[256];
    int count = 0;

    // Skip header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && count < max) {
        int id, evolves_to = 0;
        char name[MAX_NAME_LEN], type[MAX_TYPE_LEN], evo_str[10];

        sscanf(line, "%d,%49[^,],%19[^,],%9s", &id, name, type, evo_str);
        if (strcmp(evo_str, "") != 0)
            evolves_to = atoi(evo_str);

        pokedex[count].id = id;
        strncpy(pokedex[count].name, name, MAX_NAME_LEN);
        strncpy(pokedex[count].type, type, MAX_TYPE_LEN);
        pokedex[count].evolves_to = evolves_to;
        count++;
    }

    fclose(file);
    return count;
}

Pokemon* findPokemonByID(Pokemon* pokedex, int size, int id) {
    for (int i = 0; i < size; i++) {
        if (pokedex[i].id == id)
            return &pokedex[i];
    }
    return NULL;
}

void evolvePokemon(Pokemon* p, Pokemon* pokedex, int size) {
    if (p->evolves_to == 0) {
        printf("%s cannot evolve.\n", p->name);
        return;
    }

    Pokemon* evolved = findPokemonByID(pokedex, size, p->evolves_to);
    if (evolved) {
        printf("%s evolved into %s!\n", p->name, evolved->name);
    } else {
        printf("Evolution ID %d not found.\n", p->evolves_to);
    }
}

void printFirstN(Pokemon* pokedex, int n) {
    for (int i = 0; i < n; i++) {
        printf("%03d: %-12s (%s)", pokedex[i].id, pokedex[i].name, pokedex[i].type);
        if (pokedex[i].evolves_to != 0)
            printf(" → %d", pokedex[i].evolves_to);
        printf("\n");
    }
}

void searchByType(Pokemon* pokedex, int size, const char* type) {
    printf("Searching for Pokemon of type '%s':\n", type);
    for (int i = 0; i < size; i++) {
        if (strcasecmp(pokedex[i].type, type) == 0) {
            printf("%03d: %s\n", pokedex[i].id, pokedex[i].name);
        }
    }
}

int main() {
    Pokemon pokedex[MAX_POKEMON];
    int total = loadPokedex("pokedex.csv", pokedex, MAX_POKEMON);
    if (total == 0) {
        return 1;
    }

    printf("== Loaded %d Pokémon ==\n\n", total);
    printFirstN(pokedex, 10);
    printf("\n");

    Pokemon* pikachu = findPokemonByID(pokedex, total, 25);
    if (pikachu) {
        printf("Before evolution: %s\n", pikachu->name);
        evolvePokemon(pikachu, pokedex, total);
    }

    printf("\n");
    searchByType(pokedex, total, "Water");

    return 0;
}
