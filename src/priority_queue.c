#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"

PriorityQueue* create_priority_queue() {
    PriorityQueue *pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->head = NULL;
    return pq;
}

void enqueue(PriorityQueue *pq, TreeNode *tree_node) {
    PriorityQueueNode *new_node = (PriorityQueueNode*)malloc(sizeof(PriorityQueueNode));
    new_node->tree_node = tree_node;
    new_node->next = NULL;

    if (pq->head == NULL || tree_node->frequency < pq->head->tree_node->frequency) {
        new_node->next = pq->head;
        pq->head = new_node;
    } else {
        PriorityQueueNode *current = pq->head;
        while (current->next != NULL && current->next->tree_node->frequency <= tree_node->frequency) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

TreeNode* dequeue(PriorityQueue *pq) {
    if (pq->head == NULL) return NULL;

    PriorityQueueNode *temp = pq->head;
    TreeNode *tree_node = temp->tree_node;
    pq->head = pq->head->next;
    free(temp);

    return tree_node;
}

int is_priority_queue_empty(PriorityQueue *pq) {
    return (pq->head == NULL);
}

void free_priority_queue(PriorityQueue *pq) {
    while (!is_priority_queue_empty(pq)) {
        dequeue(pq);
    }
    free(pq);
}