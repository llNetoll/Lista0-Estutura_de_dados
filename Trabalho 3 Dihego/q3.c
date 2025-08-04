#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int chave;
    int altura;
    struct Node *esq, *dir;
} Node;

// Funções auxiliares
int max(int a, int b) {
    return a > b ? a : b;
}

int altura(Node *n) {
    return n ? n->altura : -1;
}

Node* criarNo(int chave) {
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->chave = chave;
    novo->altura = 0;
    novo->esq = novo->dir = NULL;
    return novo;
}

// Rotações AVL
Node* rotacaoDireita(Node* y) {
    Node* x = y->esq;
    Node* T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = 1 + max(altura(y->esq), altura(y->dir));
    x->altura = 1 + max(altura(x->esq), altura(x->dir));

    return x;
}

Node* rotacaoEsquerda(Node* x) {
    Node* y = x->dir;
    Node* T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = 1 + max(altura(x->esq), altura(x->dir));
    y->altura = 1 + max(altura(y->esq), altura(y->dir));

    return y;
}

int getBalance(Node* n) {
    return n ? altura(n->esq) - altura(n->dir) : 0;
}

// Inserção AVL
Node* inserir(Node* raiz, int chave) {
    if (!raiz) return criarNo(chave);
    if (chave < raiz->chave)
        raiz->esq = inserir(raiz->esq, chave);
    else if (chave > raiz->chave)
        raiz->dir = inserir(raiz->dir, chave);
    else
        return raiz; // duplicado, ignora

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int balance = getBalance(raiz);

    if (balance > 1 && chave < raiz->esq->chave)
        return rotacaoDireita(raiz);
    if (balance < -1 && chave > raiz->dir->chave)
        return rotacaoEsquerda(raiz);
    if (balance > 1 && chave > raiz->esq->chave) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    if (balance < -1 && chave < raiz->dir->chave) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Função auxiliar para remover o menor nó da direita (usado na remoção)
Node* minValor(Node* n) {
    Node* atual = n;
    while (atual->esq) atual = atual->esq;
    return atual;
}

// Remoção AVL
Node* remover(Node* raiz, int chave) {
    if (!raiz) return inserir(raiz, chave); // transforma em inserção

    if (chave < raiz->chave)
        raiz->esq = remover(raiz->esq, chave);
    else if (chave > raiz->chave)
        raiz->dir = remover(raiz->dir, chave);
    else {
        if (!raiz->esq || !raiz->dir) {
            Node* temp = raiz->esq ? raiz->esq : raiz->dir;
            free(raiz);
            return temp;
        } else {
            Node* temp = minValor(raiz->dir);
            raiz->chave = temp->chave;
            raiz->dir = remover(raiz->dir, temp->chave);
        }
    }

    if (!raiz) return raiz;

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int balance = getBalance(raiz);

    if (balance > 1 && getBalance(raiz->esq) >= 0)
        return rotacaoDireita(raiz);
    if (balance > 1 && getBalance(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    if (balance < -1 && getBalance(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);
    if (balance < -1 && getBalance(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Impressão em ordem com altura
void imprimirEmOrdem(Node* raiz, FILE* out) {
    if (!raiz) return;
    imprimirEmOrdem(raiz->esq, out);
    fprintf(out, "%d (%d) ", raiz->chave, raiz->altura);
    imprimirEmOrdem(raiz->dir, out);
}

// Liberação da memória
void liberarArvore(Node* raiz) {
    if (!raiz) return;
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}

// ======================
//        MAIN
// ======================
int main() {
    FILE *in = fopen("L2Q3.in", "r");
    FILE *out = fopen("L2Q3.out", "w");

    if (!in || !out) {
        printf("Erro ao abrir arquivos.\n");
        return 1;
    }

    char linha[1024];
    while (fgets(linha, sizeof(linha), in)) {
        Node* raiz = NULL;
        char tipo;
        int chave;
        char* token = strtok(linha, " \n");

        while (token) {
            tipo = token[0];
            token = strtok(NULL, " \n");
            if (!token) break;
            chave = atoi(token);

            if (tipo == 'a')
                raiz = inserir(raiz, chave);
            else if (tipo == 'r')
                raiz = remover(raiz, chave);

            token = strtok(NULL, " \n");
        }

        imprimirEmOrdem(raiz, out);

        // Verifica se há mais linhas (para evitar \n extra)
        long pos = ftell(in);
        if (fgets(linha, sizeof(linha), in)) {
            fseek(in, pos, SEEK_SET);
            fprintf(out, "\n");
        }

        liberarArvore(raiz);
    }

    fclose(in);
    fclose(out);
    return 0;
}
