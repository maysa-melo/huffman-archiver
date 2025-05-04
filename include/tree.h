#ifndef TREE_H
#define TREE_H

// Estrutura de um nó da árvore de Huffman
typedef struct TreeNode {
    unsigned char character; // Caractere representado (se folha)
    unsigned int frequency; // Frequência do caractere
    struct TreeNode *left, *right; // Ponteiros para filhos esquerdo e direito
} TreeNode;

// Cria um novo nó com caractere e frequência
TreeNode* create_tree_node(unsigned char character, unsigned int frequency);

// Constrói a árvore de Huffman usando uma tabela de frequências
TreeNode* build_huffman_tree(unsigned int freq[]);

// Gera os códigos binários para cada caractere com base na árvore de Huffman
void generate_codes(TreeNode *root, char *code, int depth, char **codes);

// Libera a memória usada pela árvore de Huffman
void free_huffman_tree(TreeNode *root);

#endif