#ifndef TREE_H
#define TREE_H

typedef struct TreeNode {
    unsigned char character;
    unsigned int frequency; 
    struct TreeNode *left, *right; 
} TreeNode;

TreeNode* create_tree_node(unsigned char character, unsigned int frequency);

TreeNode* build_huffman_tree(unsigned int freq[]);

void generate_codes(TreeNode *root, char *code, int depth, char **codes);

void free_huffman_tree(TreeNode *root);

#endif