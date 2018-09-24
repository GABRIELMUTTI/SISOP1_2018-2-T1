
#include <stdio.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/scheduler.h"

extern int first_time;

int ccreate (void* (*start)(void*), void *arg, int prio) {

	if (first_time)
	{
		initialize_scheduler_main();
	}

	TCB_t* newTcb = create_tcb(start, arg, prio);

	put_ready(newTcb); 
	check_preemption(newTcb);

	return newTcb->tid;
}

int csetprio(int tid, int prio) {
	return -1;
}

int cyield(void) {
	FirstFila2(executingQueue);
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);

	swapcontext(&executing->context, &get_scheduler()->context);
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


