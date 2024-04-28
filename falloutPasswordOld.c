#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodeStr {
    char str[256];
    struct nodeStr *prox;
} NodeStr;

int similaridade(char *, char *);
int similaridadeAcumulada(char **, int, int);
int indMaiorSimilaridade(int *, int);
NodeStr *criaLista();
NodeStr *insereLista(NodeStr *, char *);
NodeStr *retiraLista(NodeStr *, char *);
void imprimeLista(NodeStr *);
NodeStr *liberaLista(NodeStr *);
NodeStr *criaListaFromFileName(const char *);
int listaLen(NodeStr *);
char **lista2Vetor(NodeStr *, int *);
void liberaVetor(char **, int);
void imprimeVetor(char **, int);
void troca(char **, char **);

int main() {
    int n, k, t = 0, sim = 0, sim2, indRecomendado;
    int i;
    NodeStr *lista = criaListaFromFileName("senhas.txt");
    char **strs = lista2Vetor(lista, &n);
    int *similaridadeAcumuladas = (int *)malloc(sizeof(int) * n);
    char tentativas[4][256];
    liberaLista(lista);
    k = n - 1;
    while (k > 0 && t < 4) {
        for (i = 0; i <= k; i++)
            similaridadeAcumuladas[i] = similaridadeAcumulada(strs, i, k + 1);
        indRecomendado = indMaiorSimilaridade(similaridadeAcumuladas, k + 1);
        printf("Possibilidades tentativa %d:\n", t);
        imprimeVetor(strs, k + 1);
        printf("\nRecomendado tentar %s\n", strs[indRecomendado]);
        printf("Tente uma palavra\n");
        scanf("%s", tentativas[t]);
        printf("Quantas iguais a senha?\n");
        scanf("%d", &sim);
        if (sim == strlen(tentativas[t]))
            break;
        for (i = 0; i <= k; i++) {
            sim2 = similaridade(tentativas[t], strs[i]);
            while (sim2 != sim && k >= i) {
                troca(&strs[i], &strs[k]);
                k--;
                sim2 = similaridade(tentativas[t], strs[i]);
            }
        }
        t++;
    }
    free(similaridadeAcumuladas);
    liberaVetor(strs, n);
    printf("A senha eh %s\n", tentativas[t]);
    return 0;
}

void troca(char **s1, char **s2) {
    char *str = *s1;
    *s1 = *s2;
    *s2 = str;
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

int indMaiorSimilaridade(int *v, int tam) {
    int i, indMaior = 0;
    for (i = 0; i < tam; i++)
        if (v[i] > v[indMaior])
            indMaior = i;
    return indMaior;
}

NodeStr *criaLista() {
    return NULL;
}

NodeStr *insereLista(NodeStr *lista, char *str) {
    NodeStr *no = (NodeStr *)malloc(sizeof(NodeStr));
    strcpy(no->str, str);
    no->prox = lista;
    return no;
}

NodeStr *retiraLista(NodeStr *lista, char *str) {
    NodeStr *tmp = lista, *ant = NULL;
    while (tmp && strcmp(tmp->str, str) != 0) {
        ant = tmp;
        tmp = tmp->prox;
    }
    if (tmp) {
        if (ant == NULL)
            lista = lista->prox;
        else
            ant->prox = tmp->prox;
        free(tmp);
    }
    return lista;
}

void imprimeLista(NodeStr *lista) {
    while (lista) {
        printf("%s\n", lista->str);
        lista = lista->prox;
    }
}

NodeStr *liberaLista(NodeStr *lista) {
    NodeStr *tmp = NULL;
    while (lista) {
        tmp = lista;
        lista = lista->prox;
        free(tmp);
    }
    return lista;
}

NodeStr *criaListaFromFileName(const char *fname) {
    NodeStr *lista = criaLista();
    char str[256];
    FILE *arq = fopen(fname, "r");
    while (fscanf(arq, "%s", str) != EOF)
        lista = insereLista(lista, str);
    fclose(arq);
    return lista;
}

int listaLen(NodeStr *lista) {
    int l = 0;
    while (lista) {
        lista = lista->prox;
        l++;
    }
    return l;
}

char **lista2Vetor(NodeStr *lista, int *size) {
    int i, l = listaLen(lista), ls = strlen(lista->str);
    *size = l;
    char **strs = (char **)malloc(l * sizeof(char *));
    for (i = 0; i < l; i++) {
        strs[i] = (char *)malloc(ls * sizeof(char));
        strcpy(strs[i], lista->str);
        lista = lista->prox;
    }
    return strs;
}

void liberaVetor(char **strs, int size) {
    int i;
    for (i = 0; i < size; i++)
        free(strs[i]);
    free(strs);
}

void imprimeVetor(char **strs, int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%s\n", strs[i]);
}