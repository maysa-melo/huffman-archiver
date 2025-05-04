#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "tree.h"

// Estrutura de um nó da fila de prioridade, que guarda um ponteiro para um nó da árvore de Huffman.
typedef struct PriorityQueueNode {
    TreeNode *tree_node; // Nó da árvore com frequência e caractere
    struct PriorityQueueNode *next; // Próximo da fila
} PriorityQueueNode;

// Estrutura principal da fila de prioridade (aponta para o início da fila)
typedef struct {
    PriorityQueueNode *head;
} PriorityQueue;

// Cria uma fila de prioridade vazia
PriorityQueue* create_priority_queue();

// Insere um nó na fila em ordem crescente de frequência
void enqueue(PriorityQueue *pq, TreeNode *tree_node);

// Remove o nó de menor frequência (maior prioridade)
TreeNode* dequeue(PriorityQueue *pq);

// Verifica se a fila está vazia
int is_priority_queue_empty(PriorityQueue *pq);

// Libera toda a memória usada pela fila
void free_priority_queue(PriorityQueue *pq);

#endif