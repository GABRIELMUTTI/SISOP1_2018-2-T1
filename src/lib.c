
#include <stdio.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/scheduler.h"


int ccreate (void* (*start)(void*), void *arg, int prio) {
	TCB_t* newTcb;
	if(first_time)
	{
		first_time = 0;
		initialize_scheduler_main();
	}
	newTcb = create_tcb(start,arg,prio);
	put_ready(newTcb);

	return newTcb->tid;
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


