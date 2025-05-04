CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c99
SRC = src/main.c src/huffman.c src/priority_queue.c src/tree.c
OBJ = $(SRC:.c=.o)
EXEC = huffman-archiver

# Regra padrão: compilar o projeto
all: $(EXEC)

# Regra para compilar o executável
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Regras para compilar os arquivos .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados
clean:
	rm -f $(EXEC) $(OBJ)

# Regra para testar a compressão
test_compress: $(EXEC)
	./$(EXEC) 1 input.txt output.huff

# Regra para testar a descompressão
test_decompress: $(EXEC)
	./$(EXEC) 2 output.huff output_dec.txt

# Regra para rodar os testes de compressão e descompressão
test: test_compress test_decompress