#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib

all: exemplo_modificado

exemplo_modificado: exemplo_modificado.c $(LIB_DIR)/libcthread.a
	$(CC) -o exemplo_modificado exemplo_modificado.c -L$(LIB_DIR) -lcthread -Wall

clean:
	rm -rf exemplo_modificado *.o *~




