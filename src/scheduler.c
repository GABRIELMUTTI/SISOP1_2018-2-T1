#include "../include/scheduler.h"

void initialize_scheduler_main() {

    // Create main's TCB.
	TCB_t* mainTcb = create_tcb(0);
	getcontext(&mainTcb->context);

    // Create scheduler's TCB.
	TCB_t* schedulerTcb = create_tcb(0);

	char stack[STACK_SIZE];

	getcontext(&schedulerTcb->context);
	schedulerTcb->context.uc_stack.ss_sp = stack;
    schedulerTcb->context.uc_stack.ss_size = sizeof(stack);
    schedulerTcb->context.uc_link = &mainTcb->context;
	makecontext(&schedulerTcb->context, &schedule, 0);
}

void schedule()
{
    // FALTA IMPLEMENTAR.
}

void put_ready(TCB_t* tcb)
{
	switch(tcb->prio)
		{ 
			case 0:
                // Put in queue 0.
                break;
			case 1:
                // Put in queue 1.
                break;
			case 2:
                // Put in queue 2.
                break;
		}
	tcb->state = PROCST_APTO;
	check_preemption(tcb);
}

void check_preemption(TCB_t* tcb)
{
	FirstFila2(executingQueue);
	TCB_t* executing = (TCB_t*)GetAtIteratorFila2(executingQueue);
	if(executing->prio < tcb->prio)
    {
        TrocaExecutando(tcb);
    }
		
}