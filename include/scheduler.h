#ifndef	__SCHEDULER_H__
#define	__SCHEDULER_H__

#include "../include/cdata.h"
#include "../include/support.h"

int first_time;
const int STACK_SIZE = 8192;

PFILA2 ready0Queue;
PFILA2 ready1Queue;
PFILA2 ready2Queue;
PFILA2 executingQueue;


void initialize_scheduler_main();
TCB_t* create_tcb(int prio);
TCB_t* get_scheduler();
void schedule();
void put_ready(TCB_t* tcb);
void check_preemption(TCB_t* tcb);

#endif