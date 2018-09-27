#include "../include/scheduler.h"

void initialize_scheduler_main() {

    // Create main's TCB.
	create_main_tcb();
	
    // Create scheduler's TCB.
	create_tcb(&schedule, 0, 0);

	CreateFila2(ready0Queue);
	CreateFila2(ready1Queue);
	CreateFila2(ready2Queue);
	CreateFila2(blockedQueue);
	CreateFila2(executingQueue);
	CreateFila2(tcbs);
	CreateFila2(sems);
}

void schedule()
{
	TCB_t* highestPriorityTcb = get_highest_priority_ready_tcb();
	TCB_t* scheduler = get_scheduler();

	while (highestPriorityTcb != NULL)
	{
		if (scheduler->state != PROCST_EXEC)
		{
			remove_executing();

			// Puts scheduler TCB in executing queue.
			AppendFila2(executingQueue, get_scheduler());
			get_scheduler()->state = PROCST_EXEC;
		}
		
		if (highestPriorityTcb != NULL)
		{
			execute_preemption(highestPriorityTcb);
		}

		highestPriorityTcb = get_highest_priority_ready_tcb();
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

void put_blocked(TCB_t* tcb, PFILA2 blockedQueue)
{
	AppendFila2(blockedQueue, tcb);
	tcb->state = PROCST_BLOQ;
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
	tcb->state = PROCST_EXEC;

	swapcontext(&executing->context, &tcb->context);
}



TCB_t* get_highest_priority_ready_tcb()
{
	
	
	
	if(FirstFila2(ready0Queue) == 0) //Queue not empty
		return (TCB_t*)GetAtIteratorFila2(ready0Queue);
	else
		if(FirstFila2(ready1Queue) == 0)
			return (TCB_t*)GetAtIteratorFila2(ready1Queue);
		else
			if(FirstFila2(ready2Queue) == 0)
				return (TCB_t*)GetAtIteratorFila2(ready2Queue);

	return NULL;
}

TCB_t* get_highest_priority_blocked_tcb(PFILA2 queue)
{
	TCB_t* tcb;
	tcb = get_element_of_priority(queue, 0);

	if(tcb == NULL)
		tcb = get_element_of_priority(queue, 1);
	
	if(tcb == NULL)
		tcb = get_element_of_priority(queue, 2);
	
	
	
	
	return tcb;
}

TCB_t* get_element_of_priority(PFILA2 queue, int prio)
{
	if(FirstFila2(queue) != 0) //queue empty
		return NULL;

	TCB_t* tcb;
	tcb = (TCB_t*)GetAtIteratorFila2(queue);

	if(tcb -> prio == prio)
		return tcb;
	else
		while(NextFila2(queue) != NXTFILA_ENDQUEUE)
			{
				tcb = (TCB_t*)GetAtIteratorFila2(queue);
				if(tcb -> prio == prio)
					return tcb;
			}

	return NULL; //no element of prio in queue
}