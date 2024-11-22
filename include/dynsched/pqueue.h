#ifndef __PQUEUE_H__
#define __PQUEUE_H__

// priority queue, implemented using a linked list
// the list is sorted in ascending order of priority

#include <dynsched/mem.h>

/**------------------------------------------------------------------------
 *                           PRIORITY QUEUE STRUCTURES
 *------------------------------------------------------------------------**/


typedef struct dynsched_psched_node dynsched_pqueue_node_t;
typedef int (*dynsched_pqueue_compare_func)(void *, void *);

typedef struct dynsched_psched_node {
    void *data;
    dynsched_pqueue_node_t *next;
    dynsched_pqueue_node_t *prev;
} dynsched_pqueue_node_t;

typedef struct {
    dynsched_pqueue_node_t *head;
    dynsched_pqueue_node_t *tail;
    dynsched_mem_size_t size;
    dynsched_mem_manager_t *mem_manager;
    dynsched_pqueue_compare_func compare_func;
} dynsched_pqueue_t;

/**------------------------------------------------------------------------
 *                           PRIORITY QUEUE FUNCTIONS
 *------------------------------------------------------------------------**/

dynsched_pqueue_t *dynsched_pqueue_create(dynsched_mem_manager_t *mem_manager);
void dynsched_pqueue_destroy(dynsched_pqueue_t *queue);

dynsched_pqueue_node_t *dynsched_pqueue_node_create(dynsched_mem_manager_t *mem_manager, void *data, int priority);
void dynsched_pqueue_node_destroy(dynsched_pqueue_node_t *node);

void dynsched_pqueue_push(dynsched_pqueue_t *queue, void *data, int priority);
void *dynsched_pqueue_pop(dynsched_pqueue_t *queue);
void *dynsched_pqueue_peek(dynsched_pqueue_t *queue);
dynsched_pqueue_node_t *dynsched_pqueue_pop_node(dynsched_pqueue_t *queue);
dynsched_pqueue_node_t *dynsched_pqueue_peek_node(dynsched_pqueue_t *queue);
void dynsched_pqueue_sort(dynsched_pqueue_t *queue);
void dynsched_pqueue_begin(dynsched_pqueue_t *queue);
void dynsched_pqueue_next(dynsched_pqueue_t *queue);
void dynsched_pqueue_end(dynsched_pqueue_t *queue);

/**------------------------------------------------------------------------
 *                           PRIORITY QUEUE INTERNAL FUNCTIONS
 *------------------------------------------------------------------------**/

void dynsched_pqueue_insert(dynsched_pqueue_t *queue, dynsched_pqueue_node_t *node);
void dynsched_pqueue_remove(dynsched_pqueue_t *queue, dynsched_pqueue_node_t *node);
void dynsched_pqueue_swap(dynsched_pqueue_node_t *node1, dynsched_pqueue_node_t *node2);

#endif  // __PQUEUE_H__