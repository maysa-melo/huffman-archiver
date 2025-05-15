#include <stdio.h>
#include "huffman.h"

int main() {
    int choice;
    char input_filename[100], output_filename[100];

    printf("Codificacao de Huffman\n");
    printf("1 - Compactar arquivo\n");
    printf("2 - Descompactar arquivo\n");
    printf("Escolha uma opção: ");
    scanf("%d", &choice);

    printf("Digite o nome do arquivo de entrada (sem caminho): ");
    scanf("%s", input_filename);
    printf("Digite o nome do arquivo de saida (sem caminho): ");
    scanf("%s", output_filename);

    if (choice == 1) {
        compress_file(input_filename, output_filename);
    } else if (choice == 2) {
        decompress_file(input_filename, output_filename);
    } else {
        printf("Opção invalida!\n");
    }

    return 0;
}
