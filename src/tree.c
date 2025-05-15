#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "priority_queue.h"

TreeNode* create_tree_node(unsigned char character, unsigned int frequency) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    node->character = character;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

TreeNode* build_huffman_tree(unsigned int freq[]) {
    PriorityQueue *pq = create_priority_queue();

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            enqueue(pq, create_tree_node((unsigned char)i, freq[i]));
        }
    }

    while (pq->head != NULL && pq->head->next != NULL) {
        TreeNode *left = dequeue(pq);
        TreeNode *right = dequeue(pq);

        TreeNode *parent = create_tree_node('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        enqueue(pq, parent);
    }

    TreeNode *root = dequeue(pq);
    free_priority_queue(pq);
    return root;
}

void generate_codes(TreeNode *root, char *code, int depth, char **codes) {
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL) {
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

void free_huffman_tree(TreeNode *root) {
    if (root == NULL) return;
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}
