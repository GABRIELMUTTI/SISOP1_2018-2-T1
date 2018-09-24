
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
		first_time = 0;
		initialize_scheduler_main();
	}

	TCB_t* newTcb = create_tcb(start, arg, prio);

	put_ready(newTcb); 
	check_preemption(newTcb);

	return newTcb->tid;
}

int csetprio(int tid, int prio) {

	FirstFila2(executingQueue);
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);

	int oldPrio = executing->prio;
	executing->prio = prio;

	if (oldPrio > prio)
	{
		TCB_t* highestPriorityTcb = get_highest_priority_tcb();

		if (highestPriorityTcb != NULL && highestPriorityTcb->prio > prio)
		{
			execute_preemption(highestPriorityTcb);
		}
	}

	return 0;
}

int cyield(void) {
	FirstFila2(executingQueue);
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);

	put_ready(executing);

	swapcontext(&executing->context, &get_scheduler()->context);

	return 0;
}

int cjoin(int tid) {
	
	FirstFila2(executingQueue);
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);

	put_blocked(executing, blockedQueue);

	swapcontext(&executing->context, &get_scheduler()->context);

	return 0;
}

int csem_init(csem_t *sem, int count) {
	sem->count = count;
	CreateFila2(sem->fila);

	return 0;
}

int cwait(csem_t *sem) {
	
	if (sem->count <= 0)
	{
		FirstFila2(executingQueue);
		TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);

		put_blocked(executing, sem->fila);

		swapcontext(&executing->context, &get_scheduler()->context);
	}
	else
	{
		sem->count--;
	}

	return 0;
}

int csignal(csem_t *sem) {
	sem->count++;

	FirstFila2(executingQueue);
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);

	TCB_t* blockedTcb = get_highest_priority_tcb(sem->fila);

	put_ready(blockedTcb);

	swapcontext(&executing->context, &get_scheduler()->context);

	return 0;
}

int cidentify (char *name, int size) {
	strncpy (name, "Sergio Cechin - 2017/1 - Teste de compilacao.", size);
	return 0;
}


