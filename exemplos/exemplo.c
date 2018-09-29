
/*
 *	Programa de exemplo de uso da biblioteca cthread
 *
 *	Versão 1.0 - 14/04/2016
 *
 *	Sistemas Operacionais I - www.inf.ufrgs.br
 *
 */

#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

void* func0(void *arg) {
		
	printf("Eu sou a thread ID0 imprimindo %d\n", *((int *)arg));	
	return;
}

void* func1(void *arg) {
	printf("Eu sou a thread ID1 imprimindo %d\n", *((int *)arg));
}

void* func2(void *arg) {
	
	printf("Eu sou a thread ID2 imprimindo %d\n", *((int *)arg));
}

int main(int argc, char *argv[]) {
	int	id0, id1, id2;
	int i = 0;
	
	id0 = ccreate(func0, (void *)&i, 0);
	id1 = ccreate(func1, (void *)&i, 0);
	id2 = ccreate(func2, (void *)&i, 0);
	
	
	printf("Eu sou a main após a criação de ID0 e ID1\n");

	cjoin(id0);
	cjoin(id1);
	cjoin(id2);

	printf("Eu sou a main voltando para terminar o programa\n");
}

