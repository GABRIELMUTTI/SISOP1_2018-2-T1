
#include <stdio.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"


int ccreate (void* (*start)(void*), void *arg, int prio) {

	testaseprimeiravez

	contextotcb  = create_TCB(prio)	
	makecontext( contextotcb.context , start,arg
	bota_apto(contextotcb) 
	return contextotcb.tid;
}

void bota_apto(TCB_t* contextotcb)
{
	switch(contectotcb->prio)
		{ 
			case 0
			case 1
			case 2
		}
	contextotcb.state = 1;
	check_preempsao(contextotcb);
}

void check_preempsao(TCB_t* novo_tcb )
{
	FirstFila2(filaexec);
	TCB_t* tcb = (TCB_t*)GetAtIteratorFila2(filaexec);
	if(tcb->prio < novo_tcb->prio) 
		TrocaExecutando(novo_tcb);
}

void TrocaExecutando(TCB_t* novo_tcb)
{
	FirstFila2(filaexec);
	TCB_t* tcb = (TCB_t*)GetAtIteratorFila2(filaexec);
	swapcontext(&tcb->context, novo_tcp->context);
	botar_apto(tcb);
	botar_exec(novo_tcb);
}

void botar_exec(TCB_t* novo_tcb)
{
	FirstFila2(filaapto);
	DeleteAtIteratorFila2(PFILA2 filaapto);
	FirstFila2(filaexec);
	DeleteAtIteratorFila2(PFILA2 filaexec);
	
}


int csetprio(int tid, int prio) {
	return -1;
}

int cyield(void) {
	return -1;
}

int cjoin(int tid) {
	return -1;
}

int csem_init(csem_t *sem, int count) {
	return -1;
}

int cwait(csem_t *sem) {
	return -1;
}

int csignal(csem_t *sem) {
	return -1;
}

int cidentify (char *name, int size) {
	strncpy (name, "Sergio Cechin - 2017/1 - Teste de compilacao.", size);
	return 0;
}


