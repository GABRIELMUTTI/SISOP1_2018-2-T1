#include "../include/scheduler.h"

int first_time = 1;
const int STACK_SIZE = 8192;
int num_tcbs = 0;

void initialize_scheduler_main() 
{
	ready0Queue = create_queue();
	ready1Queue = create_queue();
	ready2Queue = create_queue();
	blockedQueue = create_queue();
	executingQueue = create_queue();
	tcbs = create_queue();
	sems = create_queue();
	
    // Create main's TCB.
	create_main_tcb();
	
    // Create scheduler's TCB.
	create_scheduler_tcb(&schedule, 0, 2);
}

PFILA2 create_queue()
{
	PFILA2 queue = (PFILA2)malloc(sizeof(FILA2));
	int error = CreateFila2(queue);
	if (error != 0)
	{
		printf("Error in queue creation.\n");
		return NULL;
	}
	
	return queue;
}

void schedule()
{	
	DEBUG_PRINT("*** SCHEDULING (FIRST TIME) ***\n\n", 0);
	
	on_thread_end();
	TCB_t* highestPriorityTcb = get_highest_priority_ready_tcb();
	

	
	while (highestPriorityTcb != NULL)
	{	
		DEBUG_PRINT("*** SCHEDULING:\n\t tid: %d,\tprio: %d ***\n\n", highestPriorityTcb->tid, highestPriorityTcb->prio);


		
		
		if (highestPriorityTcb != NULL)
		{
			execute_preemption(highestPriorityTcb);
		}

		on_thread_end();
		highestPriorityTcb = get_highest_priority_ready_tcb();
		
	}
		
		
	
	
}

void on_thread_end()
{
	// If executing thread ended.
	if (FirstFila2(executingQueue) == 0)
	{
		FirstFila2(executingQueue);
		TCB_t* finished_tcb = (TCB_t*)GetAtIteratorFila2(executingQueue);
		finished_tcb->state = PROCST_TERMINO;
		deblock_threads();
		remove_executing();
		
		
	}
}



void deblock_threads()
{
	FirstFila2(executingQueue);
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);
	
	int end = FirstFila2(blockedQueue);
	int found_blocked = 0;
	while (!end && !found_blocked)
	{
		
		TCB_t* blocked_tcb = (TCB_t*)GetAtIteratorFila2(blockedQueue);
		if (((TCB_data_t*)blocked_tcb->data)->tid_joined == executing->tid)
		{
			put_ready(blocked_tcb);
			DeleteAtIteratorFila2(blockedQueue);
			found_blocked = 1;

			DEBUG_PRINT("*** DEBLOCKING:\n\t tid: %d,\tprio: %d ***\n\n", blocked_tcb->tid, blocked_tcb->prio);
		}

		end = NextFila2(blockedQueue);
	}
}

void remove_executing()
{
	FirstFila2(executingQueue);
	DeleteAtIteratorFila2(executingQueue);
}

void remove_highest_priority_ready_tcb()
{
	if(FirstFila2(ready0Queue) == 0)
	{
		DeleteAtIteratorFila2(ready0Queue);
	}	
	else
	{
		if(FirstFila2(ready1Queue) == 0)
		{
			DeleteAtIteratorFila2(ready1Queue);
		}		
		else
		{
			if(FirstFila2(ready2Queue) == 0)
			{
				DeleteAtIteratorFila2(ready2Queue);
			}		
		}
	}
}

TCB_t* get_scheduler()
{
	FirstFila2(tcbs);
	NextFila2(tcbs);
	
	return (TCB_t*)GetAtIteratorFila2(tcbs);
}
TCB_t* get_main()
{
	FirstFila2(tcbs);
	
	return (TCB_t*)GetAtIteratorFila2(tcbs);
}

TCB_t* create_tcb(void* (*start)(void*), void *arg, int prio)
{
	
	// Allocates stack and TCB.
	char* stack = malloc(sizeof(char) * STACK_SIZE);
	TCB_t* newTcb = (TCB_t*)malloc(sizeof(TCB_t));
	newTcb->prio = prio;
	
	newTcb->tid = num_tcbs;
	newTcb->data = malloc(sizeof(TCB_data_t));
		
	num_tcbs = num_tcbs + 1;
	// Sets up the context.
	getcontext(&newTcb->context);
	newTcb->context.uc_link = &get_scheduler()->context;
	newTcb->context.uc_stack.ss_sp = stack;
	newTcb->context.uc_stack.ss_size = STACK_SIZE;
	
	makecontext(&newTcb->context, start, 1, arg);		
	
	// Put new TCB in the TCB list.
	AppendFila2(tcbs, newTcb);
	
	return newTcb;
}

TCB_t* create_scheduler_tcb(void* (*start)(void*), void *arg, int prio)
{
	
	// Allocates stack and TCB.
	char* stack = malloc(sizeof(char) * STACK_SIZE);
	TCB_t* newTcb = (TCB_t*)malloc(sizeof(TCB_t));
	newTcb->prio = prio;
	newTcb->tid = 1;
	newTcb->data = malloc(sizeof(TCB_data_t));
	num_tcbs = num_tcbs + 1;
	// Sets up the context.
	getcontext(&newTcb->context);
	newTcb->context.uc_link = &get_main()->context;
	newTcb->context.uc_stack.ss_sp = stack;
	newTcb->context.uc_stack.ss_size = STACK_SIZE;
	
	makecontext(&newTcb->context, start, 1, arg);		
	
	// Put new TCB in the TCB list.
	AppendFila2(tcbs, newTcb);
	
	return newTcb;
}

TCB_t* create_main_tcb()
{
	TCB_t* mainTcb = (TCB_t*)malloc(sizeof(TCB_t));
	mainTcb->prio = 2;
	mainTcb->tid = 0;
	mainTcb->data = malloc(sizeof(TCB_data_t));
	num_tcbs = num_tcbs + 1;
	getcontext(&mainTcb->context);
	AppendFila2(tcbs, mainTcb);
	AppendFila2(executingQueue, mainTcb);


	return mainTcb;
}

void put_ready(TCB_t* tcb)
{
	
	if(tcb->prio == 1)
		DEBUG_PRINT("Botei %d\n",tcb->prio);

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

int tcb_exists(int tid)
{
	
	int end = FirstFila2(tcbs);
	while (!end)
	{
		TCB_t* tcb = (TCB_t*)GetAtIteratorFila2(tcbs);
		if (tcb->tid == tid)
		{
			return 1;
		}

		end = NextFila2(tcbs);
	}

	return 0;
}

int tcb_state(int tid)
{
	int end = FirstFila2(tcbs);
	while (!end)
	{
		TCB_t* tcb = (TCB_t*)GetAtIteratorFila2(tcbs);
		if (tcb->tid == tid)
		{
			return tcb->state;
		}

		end = NextFila2(tcbs);
	}

	return -1;
}

int exists_blocked_thread(int tid)
{
	int end = FirstFila2(blockedQueue);

	while (!end)
	{
		TCB_t* blocked_tcb = (TCB_t*)GetAtIteratorFila2(blockedQueue);
		if (((TCB_data_t*)blocked_tcb->data)->tid_joined == tid)
		{
			return 1;
		}

		end = NextFila2(blockedQueue);
	}

	return 0;
}

void put_blocked(PFILA2 queue)
{
	FirstFila2(executingQueue);
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);

	AppendFila2(queue, executing);
	executing->state = PROCST_BLOQ;

	remove_executing();
}

void check_preemption(TCB_t* tcb)
{
	FirstFila2(executingQueue);
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);
		
	if(executing->prio > tcb->prio)
    {
        execute_preemption(tcb);
    }
}


void execute_preemption(TCB_t* tcb)
{	
	remove_highest_priority_ready_tcb();

	if (FirstFila2(executingQueue) == 0)
	{
		DEBUG_PRINT("*** EXECUTING_PREEMPTION:\n\t tid: %d,\tprio: %d ***\n\n", tcb->tid, tcb->prio);
		TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);
		DeleteAtIteratorFila2(executingQueue);
		put_ready(executing);

		AppendFila2(executingQueue, tcb);
		tcb->state = PROCST_EXEC;
		
		swapcontext(&executing->context, &tcb->context);
	}
	else
	{
		DEBUG_PRINT("*** PUTTING IN EXECUTION:\n\t tid: %d,\tprio: %d ***\n\n", tcb->tid, tcb->prio);
		AppendFila2(executingQueue, tcb);
		tcb->state = PROCST_EXEC;
		
		swapcontext(&get_scheduler()->context, &tcb->context);
	}
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
