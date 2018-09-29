
#include <stdio.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/scheduler.h"

int ccreate (void* (*start)(void*), void *arg, int prio) {
	
	if (first_time)
	{
		first_time = 0;
		initialize_scheduler_main();
		
	}
	
	TCB_t* newTcb = create_tcb(start, arg, prio);
	DEBUG_PRINT("*** CCREATE:\n\t tid: %d,\tprio: %d ***\n\n", newTcb->tid, newTcb->prio);
	
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
		TCB_t* highestPriorityTcb = get_highest_priority_ready_tcb();

		if (highestPriorityTcb != NULL && highestPriorityTcb->prio > prio)
		{
			execute_preemption(highestPriorityTcb);
		}
	}

	return 0;
}

int cyield(void) {
	
	int retorno;
	FirstFila2(executingQueue);
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);
	DEBUG_PRINT("*** CYIELD:\n\t tid: %d\n\n", executing->tid);
	put_ready(executing);
	remove_executing();
	retorno = swapcontext(&executing->context, &get_scheduler()->context);

	return retorno;
}

int cjoin(int tid) {
	DEBUG_PRINT("*** CJOIN:\n\t tid: %d\n\n", tid);
	
	if(FirstFila2(executingQueue)!=0)printf("ERRO CJOIN\n");
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);

	if(tcb_state(tid) == PROCST_TERMINO) return 0;//thread ja terminou.

	if (tcb_exists(tid) && !exists_blocked_thread(tid))
	{
				//printf("ENTROU  %d\n");
		((TCB_data_t*)executing->data)->tid_joined = tid;
				
		put_blocked(blockedQueue);
		
		swapcontext(&executing->context, &get_scheduler()->context);
	}
	else
	{
		return -1;
	}
	
	return 0;
}

int csem_init(csem_t *sem, int count) {
	
	sem->count = count;
	
	if(CreateFila2(sem->fila) != 0)return -1;

	return 0;
}

int cwait(csem_t *sem) {
	
	if (sem->count <= 0)
	{
		FirstFila2(executingQueue);
		TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);

		put_blocked(sem->fila);

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

	TCB_t* blockedTcb = get_highest_priority_blocked_tcb(sem->fila);

	put_ready(blockedTcb);
	
	swapcontext(&executing->context, &get_scheduler()->context);

	return 0;
}

int cidentify (char *name, int size) {
	strncpy (name, "Felipe De Mello Flores, 00274712.\nGabriel Mutti Teixeira, 00261586.\n", size);
	return 0;
}


