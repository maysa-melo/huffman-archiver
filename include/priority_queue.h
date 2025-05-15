#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "tree.h"

typedef struct PriorityQueueNode {
    TreeNode *tree_node; 
    struct PriorityQueueNode *next; 
} PriorityQueueNode;

typedef struct {
    PriorityQueueNode *head;
} PriorityQueue;

PriorityQueue* create_priority_queue();

void enqueue(PriorityQueue *pq, TreeNode *tree_node);

TreeNode* dequeue(PriorityQueue *pq);

int is_priority_queue_empty(PriorityQueue *pq);

void free_priority_queue(PriorityQueue *pq);

#endif