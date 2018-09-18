
#include <stdio.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/scheduler.h"

extern int first_time;
extern const int STACK_SIZE;

int ccreate (void* (*start)(void*), void *arg, int prio) {

	if (first_time)
	{
		initialize_scheduler_main();
	}

	TCB_t* newTcb = create_tcb(prio);

	char stack[STACK_SIZE];

	getcontext(&newTcb->context);
	newTcb->context.uc_stack.ss_sp = stack;
    newTcb->context.uc_stack.ss_size = sizeof(stack);
    newTcb->context.uc_link = &get_scheduler()->context;

	makecontext(&newTcb->context , start, arg);
	put_ready(newTcb); 
	return newTcb->tid;
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


