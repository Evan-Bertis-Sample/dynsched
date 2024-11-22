/**------------------------------------------------------------------------
 *                             task.c
 *
 *  An implementation of a task.
 *
 *
 *------------------------------------------------------------------------**/

#include <dynsched/task.h>

/**------------------------------------------------------------------------
 *                           TASK CONSTRUCTOR/DESTRUCTOR
 *------------------------------------------------------------------------**/

dynsched_task_desc_t *dynsched_task_create(dynsched_mem_manager_t *mem_manager, dynsched_task_desc_t *task) {
    dynsched_task_t *new_task = (dynsched_task_t *)dynsched_mem_alloc(mem_manager, sizeof(dynsched_task_t));

    // set the task context to null, this is for the scheduler to use
    dynsched_task_desc_t new_task_desc = *task;
    // now make a copy of the name of the task, it is likely that it will be destroyed
    // after the function call
    new_task_desc.task_name = (char *)dynsched_mem_alloc(mem_manager, strlen(task->task_name) + 1);
    strcpy(new_task_desc.task_name, task->task_name);

    new_task->task = new_task_desc;
    new_task->context.sched_task_ctx = NULL;
    new_task->context.task_ctx = NULL;

    return new_task;
}

void dynsched_task_destroy(dynsched_mem_manager_t *mem_manager, dynsched_task_desc_t *task) {
    dynsched_mem_free(mem_manager, task->task_name);
    dynsched_mem_free(mem_manager, task);
}