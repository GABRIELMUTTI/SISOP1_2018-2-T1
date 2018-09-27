#ifndef	__SCHEDULER_H__
#define	__SCHEDULER_H__

#define NULL 0

#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/cthread.h"
#include <stdlib.h>

int first_time = 1;
const int STACK_SIZE = 8192;

PFILA2 ready0Queue;
PFILA2 ready1Queue;
PFILA2 ready2Queue;
PFILA2 executingQueue;

PFILA2 blockedQueue;

PFILA2 tcbs;
PFILA2 sems;


void initialize_scheduler_main();
TCB_t* create_tcb(void* (*start)(void*), void *arg, int prio);
TCB_t* create_main_tcb();
TCB_t* get_scheduler();
void schedule();
void put_ready(TCB_t* tcb);
void put_blocked(TCB_t* tcb, PFILA2 blockedQueue);
void check_preemption(TCB_t* tcb);
void execute_preemption(TCB_t* tcb);
void remove_executing();

TCB_t* get_highest_priority_ready_tcb();
TCB_t* get_highest_priority_blocked_tcb(PFILA2 queue);
TCB_t* get_element_of_priority(PFILA2 queue, int prio);
TCB_t* get_tcb(int tid);




#endif