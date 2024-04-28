#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// free the memory allocated for the string array and its strings
void freeStringArray(char **, int);
// prints the strings in the string array
void printStringArray(char **, int);
// pass &s1 and &s2 to swap the strings
void swap(char **, char **);
// gets passwords from file and sets n to how many strings were read
char **getPasswordsFromFile(const char *, int *);
// gets similarity between two strings
int getMaxSimilarity(int *, int);
// similaridade entre duas strings
int getSimilarity(char *, char *);
// similaridade de uma possível senha com as outras
int similaridadeAcumulada(char **, int, int);
// remove strings that don't have the same similarity as the attempt from the array of possible passwords
void filterBySimilarity(char *, char **, int *, int);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error, Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    char *fname = argv[1];

    int passwordsLength;

    // gets all passwords from file and converts to array of strings
    // sets n to how many strings were read
    char **passwords = getPasswordsFromFile(fname, &passwordsLength);

    // creates a list of similarities between each strings
    int *accumulatedSimilarityAmount = (int *)malloc(sizeof(int) * passwordsLength);

    // saves each string that was attempted on the terminal
    char attempt[4][256];

    int possibleCorrectAmount = passwordsLength;
    int attemptAmount = 0;
    while (possibleCorrectAmount > 1 && attemptAmount < 4) {
        // reprocess the similarities between the strings that could still be passwords
        // because array changes after each attempt
        for (int i = 0; i < possibleCorrectAmount; i++) {
            accumulatedSimilarityAmount[i] = similaridadeAcumulada(passwords, i, possibleCorrectAmount);
        }

        // gets the index of the string with the highest similarity
        int recommendedIndex = getMaxSimilarity(accumulatedSimilarityAmount, possibleCorrectAmount);

        int similarityFromGame;

        // prints the possibilities with a recommendation
        printf("Attempt number %d:\n", attemptAmount);
        printStringArray(passwords, possibleCorrectAmount);
        printf("\nTry %s\n", passwords[recommendedIndex]);
        // asks for a new attempt
        printf("Write down the attempt\n");
        scanf("%s", attempt[attemptAmount]);
        // asks its similarity given by the game
        printf("How similar did the game say the attempt was?\n");
        scanf("%d", &similarityFromGame);
        if (similarityFromGame == strlen(attempt[attemptAmount])) {
            printf("The password is %s\n", attempt[attemptAmount]);
            break;
        }

        // remove strings that don't have the same similarity as the attempt from the array of possible passwords
        filterBySimilarity(attempt[attemptAmount], passwords, &possibleCorrectAmount, similarityFromGame);

        attemptAmount++;
    }

    if (possibleCorrectAmount != 1) {
        printf("Something went wrong\n");
        free(accumulatedSimilarityAmount);
        freeStringArray(passwords, passwordsLength);
        return 1;
    }

    printf("The password is %s\n", passwords[0]);
    free(accumulatedSimilarityAmount);
    freeStringArray(passwords, passwordsLength);
    return 0;
}

void filterBySimilarity(char *attempt, char **passwords, int *possibleCorrectAmount, int similarityFromGame) {
    for (int i = 0; i < *possibleCorrectAmount; i++) {
        int similarityToAttempt = getSimilarity(attempt, passwords[i]);
        if (similarityToAttempt != similarityFromGame) {
            swap(&passwords[i], &passwords[*possibleCorrectAmount - 1]);
            (*possibleCorrectAmount)--;
            i--;
        }
    }
}

int getSimilarity(char *s1, char *s2) {
    int similarity = 0;
    for (int i = 0; s1[i] != '\0' && s2[i] != '\0'; i++) {
        if (s1[i] == s2[i]) {
            similarity++;
        }
    }
    return similarity;
}

int similaridadeAcumulada(char **strs, int indexCompared, int tam) {
    int accumulatedSimilarity = 0;
    for (int i = 0; i < tam; i++) {
        if (i != indexCompared) {
            accumulatedSimilarity += getSimilarity(strs[i], strs[indexCompared]);
        }
    }
    return accumulatedSimilarity;
}

void swap(char **s1, char **s2) {
    char *str = *s1;
    *s1 = *s2;
    *s2 = str;
}

char **getPasswordsFromFile(const char *fname, int *n) {
    FILE *file = fopen(fname, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", fname);
        exit(1);
    }

    *n = 0;

    char line[256];
    while (fscanf(file, "%s", line) != EOF) {
        (*n)++;
    }

    printf("n=%d\n", *n);

    fseek(file, 0, SEEK_SET);

    char **passwords = (char **)malloc(sizeof(char *) * (*n));
    for (int i = 0; i < *n; i++) {
        passwords[i] = (char *)malloc(sizeof(char) * 256);
        fscanf(file, "%s", passwords[i]);
    }

    char len = strlen(passwords[0]);
    for (int i = 0; i < *n; i++) {
        if (strlen(passwords[i]) != len) {
            printf("Error: all passwords must have the same length\n");
            exit(1);
        }
    }

    fclose(file);
    return passwords;
}

int getMaxSimilarity(int *array, int size) {
    int i, indexMaxSimilarity = 0;
    for (i = 0; i < size; i++) {
        if (array[i] > array[indexMaxSimilarity]) {
            indexMaxSimilarity = i;
        }
    }
    return indexMaxSimilarity;
}

void freeStringArray(char **strs, int size) {
    for (int i = 0; i < size; i++) {
        free(strs[i]);
    }
    free(strs);
}

void printStringArray(char **strs, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s\n", strs[i]);
    }
}
