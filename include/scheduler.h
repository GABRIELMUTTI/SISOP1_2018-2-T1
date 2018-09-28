#ifndef	__SCHEDULER_H__
#define	__SCHEDULER_H__

#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/cthread.h"
#include <stdlib.h>
#include <stdio.h>

extern int first_time;
extern const int STACK_SIZE;
extern int num_tcbs;

typedef struct s_TCB_data
{
	int tid_joined;
} TCB_data_t;

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
TCB_t* create_scheduler_tcb(void* (*start)(void*), void *arg, int prio);
TCB_t* get_scheduler();
TCB_t* get_main();
void schedule();
void put_ready(TCB_t* tcb);
void put_blocked(PFILA2 blockedQueue);
int exists_blocked_thread(int tid);
void check_preemption(TCB_t* tcb);
void execute_preemption(TCB_t* tcb);
void remove_executing();
PFILA2 create_queue();
int tcb_exists(int tid);

TCB_t* get_highest_priority_ready_tcb();
TCB_t* get_highest_priority_blocked_tcb(PFILA2 queue);
TCB_t* get_element_of_priority(PFILA2 queue, int prio);
TCB_t* get_tcb(int tid);




#endif
