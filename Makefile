#
# Makefile de EXEMPLO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# É NECESSARIO ADAPTAR ESSE ARQUIVO de makefile para suas necessidades.
#  1. Cuidado com a regra "clean" para não apagar o "support.o"
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "cthread"
# 

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src


all: compilacao ligacao

compilacao: $(SRC_DIR)/lib.c $(SRC_DIR)/scheduler.c
	$(CC) -c  $(SRC_DIR)/lib.c -o $(BIN_DIR)/lib.o  -Wall -g
	$(CC) -c  $(SRC_DIR)/scheduler.c -o $(BIN_DIR)/scheduler.o  -Wall -g

ligacao: $(BIN_DIR)/lib.o $(BIN_DIR)/scheduler.o $(BIN_DIR)/support.o
	mkdir -p lib	
	ar crs $(LIB_DIR)/libcthread.a $(BIN_DIR)/lib.o $(BIN_DIR)/scheduler.o $(BIN_DIR)/support.o

#regran: #dependências para a regran
#	$(CC) -o $(BIN_DIR)regran $(SRC_DIR)regran.c -Wall

clean: #precisa nao removesupport.o
	rm -rf $(LIB_DIR)/*.a
	ln $(BIN_DIR)/support.o ./
	rm -rf $(BIN_DIR)
	mkdir -p $(BIN_DIR)
	mv support.o $(BIN_DIR)/
	


