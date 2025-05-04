#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "priority_queue.h"
#include "tree.h"

#define BUFFER_SIZE 1024

// Conta quantas vezes cada byte aparece no arquivo
void count_frequencias(FILE *file, unsigned int freq[256]) {
    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, file)) {
        freq[byte]++;
    }
    rewind(file); // Volta ao início para reutilizar o arquivo depois
}

// Salva a árvore de Huffman no arquivo compactado (pré-ordem)
// Usa '1' seguido do caractere para folhas, '0' para nós internos
void save_tree(TreeNode *root, FILE *out) {
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL) {
        // Folha
        fputc('1', out);
        fputc(root->character, out);
    } else {
        fputc('0', out);
        save_tree(root->left, out);
        save_tree(root->right, out);
    }
}

// Lê a árvore salva no início do arquivo compactado
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

// Função para compactar um arquivo
void compress_file(const char *input_filename, const char *output_filename) {
    // Tabela para armazenar a frequência de cada byte
    unsigned int freq[256] = {0};

    // Matriz de códigos: cada caractere terá um código binário em formato string
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

    // Conta a frequência de cada caractere no arquivo
    count_frequencias(in, freq); 

    // Constrói a árvore de Huffman com base nas frequências
    TreeNode *root = build_huffman_tree(freq);

    // Buffer temporário para montar os códigos
    char code[256];
    // Gera os códigos binários para os caracteres
    generate_codes(root, code, 0, codes);

    // Grava a árvore no início do arquivo compactado
    save_tree(root, out);
    fputc('\n', out); // Delimitador da árvore
    
    // Variáveis auxiliares para codificação bit a bit
    unsigned char byte;
    int bit_buffer = 0, bit_count = 0;

    // Lê o arquivo original novamente para gerar a codificação binária
    while (fread(&byte, sizeof(unsigned char), 1, in)) {
        char *code = codes[byte]; // Recupera o código do caractere lido
        for (int i = 0; code[i] != '\0'; i++) {
            bit_buffer = bit_buffer << 1 | (code[i] - '0');
            bit_count++;

            // Se completamos 8 bits, escrevemos um byte no arquivo
            if (bit_count == 8) {
                fputc(bit_buffer, out);
                bit_buffer = 0;
                bit_count = 0;
            }
        }
    }

    // Escreve os bits restantes no final, se houver
    if (bit_count > 0) {
        bit_buffer <<= (8 - bit_count); // Preenche com zeros à direita
        fputc(bit_buffer, out);
    }

    // Libera memória
    for (int i = 0; i < 256; i++) {
        if (codes[i]) free(codes[i]);
    }
    free_huffman_tree(root);
    fclose(in);
    fclose(out);

    printf("Arquivo compactado com sucesso!\n");
}

// Função para descompactar um arquivo
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

    // Reconstrói a árvore de Huffman a partir do início do arquivo
    TreeNode *root = load_tree(in);
    fgetc(in); // Ler o '\n' após a árvore

    // Prepara variáveis para leitura bit a bit
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
    
    // Libera memória e fecha arquivos
    free_huffman_tree(root);
    fclose(in);
    fclose(out);

    printf("Arquivo descompactado com sucesso!\n");
}