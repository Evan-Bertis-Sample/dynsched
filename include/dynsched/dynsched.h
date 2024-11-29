#ifndef __DYNSCHED_H__
#define __DYNSCHED_H__

#include <dynsched/pqueue.h>
#include <dynsched/sched.h>
#include <dynsched/task.h>
#include <dynsched/prempt.h>

/**------------------------------------------------------------------------
 *                           PRIORITY SCHEDULER STRUCTURES
 *------------------------------------------------------------------------**/

// This is an implementation of Linux's O(1) scheduler, which is a priority-based

typedef struct {
    uint8_t num_priority_levels;
    long long time_slice;
    long long reschedule_time;  // whenever we reset priorities, we need to reschedule
    dynsched_prempt_interface_t *prempt;
} dynsched_psched_config_t;


typedef struct {
    dynsched_psched_config_t config;
    dynsched_pqueue_t *queue;
    dynsched_mem_manager_t *mem_manager;
    dynsched_prempt_interface_t *prempt;
    uint32_t (*miilis_fn)(void);
} dynsched_psched_context_t;

typedef struct {
    long long start_time;
    int priority;
    void *task_registers; // holds the task registers, for context switching
    void *task_stack;    // holds the task stack, for context switching
} dynsched_psched_task_context_t;

/**------------------------------------------------------------------------
 *                           PRIORITY SCHEDULER IMPLEMENTATION
 *------------------------------------------------------------------------**/

dynsched_interface_t *dynsched_psched_create(dynsched_mem_manager_t *mem_manager, void *config);
void dynsched_psched_destroy(dynsched_interface_t *sched);

void dynsched_psched_init(void *ctx);
void dynsched_psched_add_task(void *ctx, dynsched_task_desc_t *task);
void dynsched_psched_remove_task(void *ctx, char *task_name);
void dynsched_psched_run(void *ctx);

dynsched_task_t *dynsched_psched_begin(void *ctx);
dynsched_task_t *dynsched_psched_next(void *ctx);
dynsched_task_t *dynsched_psched_end(void *ctx);

void dynsched_psched_iterate(dynsched_interface_t *sched, void (*task_func)(dynsched_task_t *));

/**------------------------------------------------------------------------
 *                           PRIORITY SCHEDULER INTERFACE
 *------------------------------------------------------------------------**/
dynsched_interface_t dynsched_psched = {
    .sched_ctx = NULL,

    // constructor/destructor
    .create = dynsched_psched_create,
    .destroy = dynsched_psched_destroy,

    // scheduler functions
    .init = dynsched_psched_init,
    .add_task = dynsched_psched_add_task,
    .remove_task = dynsched_psched_remove_task,
    .run = dynsched_psched_run,

    // iterator functions, for the scheduler to iterate over tasks
    .begin = dynsched_psched_begin,
    .next = dynsched_psched_next,
    .end = dynsched_psched_end,
};

#endif  // __DYNSCHED_H__