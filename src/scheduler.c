#include "../include/scheduler.h"

void initialize_scheduler_main() {

    // Create main's TCB.
	create_main_tcb();
	
    // Create scheduler's TCB.
	create_tcb(&schedule, 0, 0);
}

void schedule()
{
	remove_executing();

	// Puts scheduler TCB in executing queue.
	AppendFila2(executingQueue, get_scheduler());

    TCB_t* highestPriorityTcb = get_highest_priority_tcb();

	if (highestPriorityTcb != NULL)
	{
		execute_preemption(highestPriorityTcb);
	}
}

void remove_executing()
{
	FirstFila2(executingQueue);
	DeleteAtIteratorFila2(executingQueue);
}

TCB_t* get_scheduler()
{
	FirstFila2(tcbs);
	NextFila2(tcbs);

	return (TCB_t*)GetAtIteratorFila2(tcbs);
}

TCB_t* create_tcb(void* (*start)(void*), void *arg, int prio)
{
	// Allocates stack and TCB.
	char stack[STACK_SIZE];
	TCB_t* newTcb = (TCB_t*)malloc(sizeof(TCB_t));

	// Sets up the context.
	getcontext(&newTcb->context);
	newTcb->context.uc_link = &get_scheduler()->context;
	newTcb->context.uc_stack.ss_sp = stack;
	newTcb->context.uc_stack.ss_size = sizeof(stack);
	makecontext(&newTcb->context, start, arg);

	// Put new TCB in the TCB list.
	AppendFila2(tcbs, newTcb);
	
	return newTcb;
}

TCB_t* create_main_tcb()
{
	TCB_t* mainTcb = (TCB_t*)malloc(sizeof(TCB_t));

	getcontext(&mainTcb->context);

	AppendFila2(tcbs, mainTcb);

	return mainTcb;
}

void put_ready(TCB_t* tcb)
{
	switch(tcb->prio)
		{ 
			case 0:
                AppendFila2(ready0Queue, tcb);
                break;
			case 1:
                AppendFila2(ready1Queue, tcb);
                break;
			case 2:
                AppendFila2(ready2Queue, tcb);
                break;
		}

	tcb->state = PROCST_APTO;
}

void check_preemption(TCB_t* tcb)
{
	FirstFila2(executingQueue);
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);
	if(executing->prio < tcb->prio)
    {
        execute_preemption(tcb);
    }
}


void execute_preemption(TCB_t* tcb)
{
	FirstFila2(executingQueue);
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);

	DeleteAtIteratorFila2(executingQueue);
	put_ready(executing);

	AppendFila2(executingQueue, tcb);

	swapcontext(&executing->context, &tcb->context);
}

TCB_t* get_highest_priority_tcb()
{
	TCB_t* tcb = NULL;
	
	// If queue is not empty and no errors occured:
	int queueStatus = FirstFila2(ready0Queue);
	if (queueStatus != 0)
	{
		tcb = (TCB_t*)GetAtIteratorFila2(ready0Queue);
		return tcb;
	}

	queueStatus = FirstFila2(ready1Queue);
	if (queueStatus != 0)
	{
		tcb = (TCB_t*)GetAtIteratorFila2(ready1Queue);
		return tcb;
	}

	queueStatus = FirstFila2(ready2Queue);
	if (queueStatus != 0)
	{
		tcb = (TCB_t*)GetAtIteratorFila2(ready2Queue);
		return tcb;
	}

	return tcb;
}

TCB_t* get_tcb(int tid)
{


}