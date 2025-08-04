#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

Node* createNode(int value) {
    Node *newNode = (Node*) malloc(sizeof(Node));
    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, int value) {
    if (root == NULL) return createNode(value);
    if (value < root->value)
        root->left = insert(root->left, value);
    else if (value > root->value)
        root->right = insert(root->right, value);
    return root;
}

int sum(Node* root) {
    if (root == NULL) return 0;
    return root->value + sum(root->left) + sum(root->right);
}

void preorder(Node* root, FILE* out) {
    if (root == NULL) return;
    int leftSum = sum(root->left);
    int rightSum = sum(root->right);
    fprintf(out, "%d (%d) ", root->value, rightSum - leftSum);
    preorder(root->left, out);
    preorder(root->right, out);
}

void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    FILE *in = fopen("L2Q2.in", "r");
    FILE *out = fopen("L2Q2.out", "w");
    if (!in || !out) {
        printf("Erro ao abrir os arquivos.\n");
        return 1;
    }

    char line[1000];
    while (fgets(line, sizeof(line), in)) {
        Node* root = NULL;
        char *token = strtok(line, " \n");
        while (token != NULL) {
            int value = atoi(token);
            root = insert(root, value);
            token = strtok(NULL, " \n");
        }
        preorder(root, out);
        fprintf(out, "\n");
        freeTree(root);
    }

    fclose(in);
    fclose(out);
    return 0;
}
