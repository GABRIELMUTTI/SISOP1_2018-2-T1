#ifndef	__SCHEDULER_H__
#define	__SCHEDULER_H__

#define DEBUG 0

#define DEBUG_PRINT(str, ...) \
	if (DEBUG) { printf(str, __VA_ARGS__); }


#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/cthread.h"
#include <stdlib.h>
#include <stdio.h>

extern int first_time;
extern const int STACK_SIZE;
extern int num_tcbs;

//struct para cjoin
typedef struct s_TCB_data
{
	int tid_joined;
} TCB_data_t;

//FILAS
PFILA2 ready0Queue;
PFILA2 ready1Queue;
PFILA2 ready2Queue;
PFILA2 executingQueue;
PFILA2 blockedQueue;

PFILA2 tcbs;
PFILA2 sems;

/******************************************************************************
Cria todas as filas. Cria o tcb da main e do scheduler.
Executada apenas no primeiro ccreate()
******************************************************************************/
void initialize_scheduler_main();


/******************************************************************************
Parâmetros:
	start:	ponteiro para a função que a thread executará.
	arg:	um parâmetro que pode ser passado para a thread na sua criação.
	prio:	prioridade da thread a ser criada
Retorno:
	Devolve o TCB criado.
******************************************************************************/
TCB_t* create_tcb(void* (*start)(void*), void *arg, int prio);


/******************************************************************************
	Cria o TCB da main. Prio = 2. tid = 0.
Retorno:
	TCB da main criado.
******************************************************************************/
TCB_t* create_main_tcb();


/******************************************************************************
Parâmetros:
	start:	ponteiro para a função do scheduler
	arg:	um parâmetro que pode ser passado para o scheduler
	prio:	prioridade do scheduler.
Retorno:
	TCB do scheduler criad.
******************************************************************************/
TCB_t* create_scheduler_tcb(void* (*start)(void*), void *arg, int prio);



/******************************************************************************
	Devolve o TCB do scheduler.
******************************************************************************/
TCB_t* get_scheduler();


/******************************************************************************
	Devolve o TCB da main.
******************************************************************************/
TCB_t* get_main();


/******************************************************************************
	Função do scheduler. Será criado um contexto com ela e todas as threads,
menos a main, irão ser linkados a ela.
******************************************************************************/
void schedule();


/******************************************************************************
Parâmetros:
	tcb: TCB que deverá ser posto na fila de ready de sua prioridade

******************************************************************************/
void put_ready(TCB_t* tcb);


/******************************************************************************
Parâmetros:
	blockedQueue: fila em que a thread executando irá ser posta quuando for
para o estado de bloqueada. Tanto para semáforos quanto pra cjoin().

******************************************************************************/
void put_blocked(PFILA2 blockedQueue);

/******************************************************************************
	Verifica se existe alguma thread bloqueada por cjoin para a tid passada.

Parâmetros:
	tid: tid da thread que queremos saber.
Retorno:
	Se existir      => 1
	Se nao existir  => 0
******************************************************************************/
int exists_blocked_thread(int tid);


/******************************************************************************
	Verifica se a thread passada faz preempsao na que esta executando. 
Caso faça,ja chama a execucao da preempsao.

Parâmetros:
	tcb: TCB a ser verificado.

******************************************************************************/
void check_preemption(TCB_t* tcb);

/******************************************************************************
Parâmetros:
	tcb: TCB do contexto que ira entrar no lugar da executando.
******************************************************************************/
void execute_preemption(TCB_t* tcb);


/******************************************************************************
	Apenas remove a thread que esta executando.
******************************************************************************/
void remove_executing();


/******************************************************************************
	Aloca espaço e cria uma fila.

Retorno:
	caso sucesso  => retorna a fila criad
	caso erro     => NULL
******************************************************************************/
PFILA2 create_queue();


/******************************************************************************
Parâmetros:
	tid: tid do TCB a ser verificado se existe.
Retorno:
	Se existe => 1
	Se nao existe  => 0
******************************************************************************/
int tcb_exists(int tid);


/******************************************************************************
	É chamado pelo scheduler quando uma thread termina. Troca seu estado,
tira da fila de executando, e desbloqueia alguma thread que esteja bloqueado por 
causa dela
******************************************************************************/
void on_thread_end();


/******************************************************************************
Parâmetros:
	tid: tid da thread que deseja ver o estado
Retorno:
	0: Criação; 1: Apto; 2: Execução; 3: Bloqueado; 4: Término;
******************************************************************************/
int tcb_state(int tid);


/******************************************************************************
	Bota em ready todas as thread que tinham usado cjoin na tid exxecutando.
É chamado quando ela termina.
******************************************************************************/
void deblock_threads();


/******************************************************************************

Retorno:
	Retorna a thread com maior prioridade no ready.
	Retorna NULL caso nao exista nenhuma thread em ready.
******************************************************************************/
TCB_t* get_highest_priority_ready_tcb();


/******************************************************************************

Parametros: 
	queue: fila de bloqueados.
Retorno:
	Retorna a thread com maior prioridade na fila de bloqueados passada.
	Retorna NULL caso nao exista nenhuma thread.
******************************************************************************/
TCB_t* get_highest_priority_blocked_tcb(PFILA2 queue);


/******************************************************************************

Parametros:
	queue: Fila daonde queremos o elemento.
	prio: prioridade do tcb que queremos.
Retorno:
	Retorna a primeira thread de prioridade passada que aparece na fila passada.
	Retorna NULL caso nao exista nenhuma thread dessa prioridade.
******************************************************************************/
TCB_t* get_element_of_priority(PFILA2 queue, int prio);


/******************************************************************************

Parametros:
	queue: Remove a tcb de maior prioridade da queue de bloqueados.
******************************************************************************/
void remove_highest_priority_blocked_tcb(PFILA2 queue);


/******************************************************************************
	 Remove a tcb de maior prioridade da fila de readys.
******************************************************************************/
void remove_highest_priority_ready_tcb();



/******************************************************************************

Parametro:
	tid: tid de uma thread
Retorno:
	Retorna o TCB dessa thread.
******************************************************************************/
TCB_t* get_tcb(int tid);




#endif
