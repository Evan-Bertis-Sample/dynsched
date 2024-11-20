#ifndef __DYNSCHED_H__
#define __DYNSCHED_H__

#include <dynsched/sched.h>
#include <dynsched/task.h>

/**------------------------------------------------------------------------
 *                           PRIORITY SCHEDULER STRUCTURES
 *------------------------------------------------------------------------**/


typedef struct {
    dynsched_mem_manager_t *mem_manager;
} dynsched_psched_context_t;

typedef struct {
    long long start_time;
} dynsched_psched_task_context_t;

/**------------------------------------------------------------------------
 *                           PRIORITY SCHEDULER IMPLEMENTATION
 *------------------------------------------------------------------------**/

dynsched_interface_t *dynsched_psched_create(dynsched_mem_manager_t *mem_manager);
void dynsched_psched_destroy(dynsched_interface_t *sched);

void dynsched_psched_init(void *ctx);
void dynsched_psched_add_task(void *ctx, dynsched_task_desc_t *task);
void dynsched_psched_remove_task(void *ctx, char *task_name);
void dynsched_psched_run(void *ctx);

dynsched_task_t *dynsched_psched_begin(void *ctx);
dynsched_task_t *dynsched_psched_next(void *ctx);
dynsched_task_t *dynsched_psched_end(void *ctx);

void dynsched_psched_iterate(dynsched_interface_t *sched, void (*task_func)(dynsched_task_t *));
void dynsched_psched_destroy(dynsched_interface_t *sched);

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