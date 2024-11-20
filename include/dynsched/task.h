#ifndef __TASK_H__
#define __TASK_H__

#include <stdint.h>

#include "dynsched/mem.h"

/**------------------------------------------------------------------------
 *                           TASK STRUCTURES
 *------------------------------------------------------------------------**/

/// @brief Task context
/// @note Used to hold the context of a task for operation by the scheduler, and
/// the task itself
typedef struct {
    void *sched_task_ctx;  // holds the scheduler context, for internal use,
                           // used to determine how to schedule the task
    void *task_ctx;  // holds the task context, for internal use, used to run
                     // the task
} dynsched_task_context_t;

/// @brief Task descriptor
/// @note Used to describe a task to the scheduler
typedef struct {
    void (*task_func)(void *);  // task function
    char *task_name;            // task name
    uint32_t stack_size;        // stack size
} dynsched_task_desc_t;

/// @brief Task
/// @note Used to represent a task
typedef struct {
    dynsched_task_desc_t task;        // task descriptor
    dynsched_task_context_t context;  // task context
} dynsched_task_t;

/**------------------------------------------------------------------------
 *                           TASK FUNCTIONS
 *------------------------------------------------------------------------**/

/// @brief Create a new task
/// @param task_func  The task function
/// @param task_name The task name
/// @param stack_size  The stack size
/// @param mem_manager  The memory manager, used to allocate memory for the task
/// @return The new task
dynsched_task_desc_t *dynsched_task_create(void (*task_func)(void *),
                                           char *task_name, uint32_t stack_size,
                                           dynsched_mem_manager_t *mem_manager);
/// @brief Free a task
/// @param task  The task to free
/// @param mem_manager  The memory manager, used to free memory for the task
void dynsched_task_free(dynsched_task_desc_t *task,
                        dynsched_mem_manager_t *mem_manager);

#endif  // __TASK_H__