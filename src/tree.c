#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "priority_queue.h"

// Cria um novo nó da árvore com caractere e frequência
TreeNode* create_tree_node(unsigned char character, unsigned int frequency) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    node->character = character;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

// Constrói a árvore de Huffman a partir de uma tabela de frequências
TreeNode* build_huffman_tree(unsigned int freq[]) {
    PriorityQueue *pq = create_priority_queue();

    // Para cada caractere com frequência > 0, cria um nó e insere na fila
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            enqueue(pq, create_tree_node((unsigned char)i, freq[i]));
        }
    }

    // Junta os dois nós de menor frequência repetidamente
    while (pq->head != NULL && pq->head->next != NULL) {
        TreeNode *left = dequeue(pq);
        TreeNode *right = dequeue(pq);

        // Cria um novo nó pai com a soma das frequências dos filhos
        TreeNode *parent = create_tree_node('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        enqueue(pq, parent);
    }

    TreeNode *root = dequeue(pq);
    free_priority_queue(pq);
    return root;
}

// Gera os códigos binários para cada caractere percorrendo a árvore
void generate_codes(TreeNode *root, char *code, int depth, char **codes) {
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL) {
        // Cria string com o código atual
        code[depth] = '\0';
        codes[root->character] = malloc(strlen(code) + 1);
        if (codes[root->character] != NULL) {
            strcpy(codes[root->character], code);
        }
        return;
    }

    code[depth] = '0';
    generate_codes(root->left, code, depth + 1, codes);

    code[depth] = '1';
    generate_codes(root->right, code, depth + 1, codes);
}

// Libera toda a árvore recursivamente
void free_huffman_tree(TreeNode *root) {
    if (root == NULL) return;
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}
