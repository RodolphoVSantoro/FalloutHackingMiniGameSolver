#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void liberaVetor(char **, int);
void imprimeVetor(char **, int);
void troca(char **, char **);
int findMaiorSimilaridade(int *, int);
// similaridade entre duas strings
int similaridade(char *, char *);
// similaridade de uma possível senha com as outras
int similaridadeAcumulada(char **, int, int);
char **getPasswordsFromFile(const char *, int *);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    char *fname = argv[1];

    int passwordsLength, possibleCorrect, attemptAmount = 0, sim = 0, indRecomendado;

    // gets all passwords from file and converts to array of strings
    // sets n to how many strings were read
    char **passwords = getPasswordsFromFile(fname, &passwordsLength);

    // creates a list of similarities between each strings
    int *similaridadeAcumuladas = (int *)malloc(sizeof(int) * passwordsLength);

    // saves each string that was attempted on the terminal
    char tentativas[4][256];

    possibleCorrect = passwordsLength;
    while (possibleCorrect > 0 && attemptAmount < 4) {
        // reprocess the similarities between the strings that could still be passwords
        // because array changes after each attempt
        for (int i = 0; i < possibleCorrect; i++) {
            similaridadeAcumuladas[i] = similaridadeAcumulada(passwords, i, possibleCorrect);
        }

        // gets the index of the string with the highest similarity
        indRecomendado = findMaiorSimilaridade(similaridadeAcumuladas, possibleCorrect);

        // prints the possibilities with a recommendation
        printf("Possibilidades tentativa %d:\n", attemptAmount);
        imprimeVetor(passwords, possibleCorrect);
        printf("\nRecomendado tentar %s\n", passwords[indRecomendado]);
        // asks for a new attempt
        printf("Tente uma palavra\n");
        scanf("%s", tentativas[attemptAmount]);
        // asks its similarity given by the game
        printf("Quantas iguais a senha?\n");
        scanf("%d", &sim);
        if (sim == strlen(tentativas[attemptAmount])) {
            printf("A senha eh %s(len)\n", tentativas[attemptAmount]);
            break;
        }
        // remove strings that don't have the same similarity as the attempt from the array of possible passwords
        for (int i = 0; i < possibleCorrect; i++) {
            int similarityToAttempt = similaridade(tentativas[attemptAmount], passwords[i]);
            if (similarityToAttempt != sim) {
                printf("i=%d\n", i);
                troca(&passwords[i], &passwords[possibleCorrect - 1]);
                possibleCorrect--;
                i--;
            }
        }
        if (possibleCorrect == 1) {
            printf("A senha eh %s\n", passwords[0]);
            break;
        }
        if (possibleCorrect == 0) {
            printf("Algo deu errado\n");
            break;
        }
        printf("ending loop t=%d, possibleCorrect=%d\n", attemptAmount, possibleCorrect);
        attemptAmount++;
    }
    printf("k=%d, t=%d\n", possibleCorrect, attemptAmount);
    free(similaridadeAcumuladas);
    liberaVetor(passwords, passwordsLength);
    return 0;
}

int similaridade(char *s1, char *s2) {
    int i, s = 0;
    for (i = 0; s1[i] != 0 && s2[i] != 0; i++) {
        if (s1[i] == s2[i]) {
            s++;
        }
    }
    return s;
}

int similaridadeAcumulada(char **strs, int ind, int tam) {
    int i, SA = 0;
    for (i = 0; i < tam; i++)
        if (i != ind)
            SA += similaridade(strs[i], strs[ind]);
    return SA;
}

void troca(char **s1, char **s2) {
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

    fclose(file);
    return passwords;
}

int findMaiorSimilaridade(int *v, int tam) {
    int i, indMaior = 0;
    for (i = 0; i < tam; i++)
        if (v[i] > v[indMaior])
            indMaior = i;
    return indMaior;
}

void liberaVetor(char **strs, int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(strs[i]);
    }
    free(strs);
}

void imprimeVetor(char **strs, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%s\n", strs[i]);
    }
}