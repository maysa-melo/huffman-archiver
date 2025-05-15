#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "priority_queue.h"
#include "tree.h"

#define BUFFER_SIZE 1024

void count_frequencias(FILE *file, unsigned int freq[256]) {
    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, file)) {
        freq[byte]++;
    }
    rewind(file);
}

void save_tree(TreeNode *root, FILE *out) {
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL) {
        fputc('1', out);
        fputc(root->character, out);
    } else {
        fputc('0', out);
        save_tree(root->left, out);
        save_tree(root->right, out);
    }
}

TreeNode* load_tree(FILE *in) {
    int bit = fgetc(in);
    if (bit == EOF) return NULL;

    if (bit == '1') {
        unsigned char character = fgetc(in);
        return create_tree_node(character, 0);
    } else {
        TreeNode *left = load_tree(in);
        TreeNode *right = load_tree(in);
        TreeNode *parent = create_tree_node('\0', 0);
        parent->left = left;
        parent->right = right;
        return parent;
    }
}

void compress_file(const char *input_filename, const char *output_filename) {
    unsigned int freq[256] = {0};

    char *codes[256] = {0};

    char input_path[256], output_path[256];
    snprintf(input_path, sizeof(input_path), "arquivos/%s", input_filename);
    snprintf(output_path, sizeof(output_path), "arquivos/%s", output_filename);

    FILE *in = fopen(input_path, "rb");
    FILE *out = fopen(output_path, "wb");

    if (!in || !out) {
        printf("Erro ao abrir arquivos.\n");
        exit(1);
    }

    count_frequencias(in, freq); 

    TreeNode *root = build_huffman_tree(freq);

    char code[256];
    generate_codes(root, code, 0, codes);

    save_tree(root, out);
    fputc('\n', out);
    
    unsigned char byte;
    int bit_buffer = 0, bit_count = 0;

    while (fread(&byte, sizeof(unsigned char), 1, in)) {
        char *code = codes[byte]; 
        for (int i = 0; code[i] != '\0'; i++) {
            bit_buffer = bit_buffer << 1 | (code[i] - '0');
            bit_count++;

            if (bit_count == 8) {
                fputc(bit_buffer, out);
                bit_buffer = 0;
                bit_count = 0;
            }
        }
    }

    if (bit_count > 0) {
        bit_buffer <<= (8 - bit_count); 
        fputc(bit_buffer, out);
    }

    for (int i = 0; i < 256; i++) {
        if (codes[i]) free(codes[i]);
    }
    free_huffman_tree(root);
    fclose(in);
    fclose(out);

    printf("Arquivo compactado com sucesso!\n");
}

void decompress_file(const char *input_filename, const char *output_filename) {
    char input_path[256], output_path[256];
    snprintf(input_path, sizeof(input_path), "arquivos/%s", input_filename);
    snprintf(output_path, sizeof(output_path), "arquivos/%s", output_filename);

    FILE *in = fopen(input_path, "rb");
    FILE *out = fopen(output_path, "wb");

    if (!in || !out) {
        printf("Erro ao abrir arquivos.\n");
        exit(1);
    }

    TreeNode *root = load_tree(in);
    fgetc(in);

    TreeNode *current = root;
    int c;

    c = fgetc(in);
    while (c != EOF) {
        unsigned char byte = (unsigned char)c;
        for (int i = 7; i >= 0; i--) {
            if ((byte >> i) & 1)
                current = current->right;
            else 
                current = current->left;

            if (current->left == NULL && current->right == NULL) {
                fputc(current->character, out);
                current = root;
            }
        }
        c = fgetc(in);
    }
    
    free_huffman_tree(root);
    fclose(in);
    fclose(out);

    printf("Arquivo descompactado com sucesso!\n");
}