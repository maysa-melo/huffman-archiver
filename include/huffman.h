#ifndef HUFFMAN_H
#define HUFFMAN_H

// Função que compacta um arquivo usando o algoritmo de Huffman.
// Recebe o nome do arquivo de entrada e de saída.
void compress_file(const char *input_filename, const char *output_filename);

// Função que descompacta um arquivo previamente compactado por Huffman.
// Recebe o nome do arquivo compactado e o nome para o arquivo descompactado.
void decompress_file(const char *input_filename, const char *output_filename);

#endif