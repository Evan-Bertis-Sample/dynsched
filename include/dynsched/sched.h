#ifndef __SCHED_H__
#define __SCHED_H__

#include <dynsched/mem.h>
#include <dynsched/task.h>

/**------------------------------------------------------------------------
 *                           SCHEDULER INTERFACE
 *------------------------------------------------------------------------**/

typedef struct {
    void *sched_ctx;  // holds the scheduler context, for internal use

    // constructor/destructor
    dynsched_interface_t *(*create)(dynsched_mem_manager_t *mem_manager);  // create a new scheduler
    void (*destroy)(dynsched_interface_t *sched);                          // destroy the scheduler

    // scheduler functions
    void (*init)(void *ctx);                                  // initialize the scheduler
    void (*add_task)(void *ctx, dynsched_task_desc_t *task);  // add a task to the scheduler
    void (*remove_task)(void *ctx, char *task_name);          // remove a task from the scheduler
    void (*run)(void *ctx);                                   // run the scheduler

    // iterator functions, for the scheduler to iterate over tasks
    dynsched_task_t *(*begin)(void *ctx);  // get the first task
    dynsched_task_t *(*next)(void *ctx);   // get the next task
    dynsched_task_t *(*end)(void *ctx);    // get the last task

} dynsched_interface_t;

/**------------------------------------------------------------------------
 *                           SCHEDULER FUNCTIONS
 *------------------------------------------------------------------------**/

void dynsched_isched_init(dynsched_interface_t *sched) { sched->init(sched->sched_ctx); }
void dynsched_isched_add_task(dynsched_interface_t *sched, dynsched_task_desc_t task_descriptor) {
    sched->add_task(sched->sched_ctx, &task_descriptor);
}
void dynsched_isched_remove_task(dynsched_interface_t *sched, char *task_name) {
    sched->remove_task(sched->sched_ctx, task_name);
}
void dynsched_isched_run(dynsched_interface_t *sched) { sched->run(sched->sched_ctx); }

dynsched_task_t *dynsched_isched_begin(dynsched_interface_t *sched) { return sched->begin(sched->sched_ctx); }
dynsched_task_t *dynsched_isched_next(dynsched_interface_t *sched) { return sched->next(sched->sched_ctx); }
dynsched_task_t *dynsched_isched_end(dynsched_interface_t *sched) { return sched->end(sched->sched_ctx); }

void dynsched_isched_iterate(dynsched_interface_t *sched, void (*task_func)(dynsched_task_t *)) {
    dynsched_task_t *end_task = dynsched_isched_end(sched);
    dynsched_task_t *current_task = dynsched_isched_begin(sched);
    while (current_task != end_task) {
        task_func(current_task);
        current_task = dynsched_isched_next(sched);
    }
}

void dynsched_isched_destroy(dynsched_interface_t *sched) { sched->destroy(sched); }

#endif  // __SCHED_H__