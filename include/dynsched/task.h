#ifndef __TASK_H__
#define __TASK_H__

#include <stdint.h>

#include "dynsched/mem_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/**------------------------------------------------------------------------
 *                           TASK STRUCTURES
 *------------------------------------------------------------------------**/

/// @brief Task context
/// @note Used to hold the context of a task for operation by the scheduler, and
/// the task itself
typedef struct {
    void *sched_task_ctx;  // holds the scheduler context, for internal use,
                           // used to determine how to schedule the task
    void *task_ctx;        // holds the task context, for internal use, used to run
                           // the task
} dynsched_task_context_t;

/// @brief Task descriptor
/// @note Used to describe a task to the scheduler
typedef struct {
    void (*task_func)(void *);  // task function
    char *task_name;            // task name
    uint32_t stack_size;        // stack size
    uint32_t register_size;     // size of all the relevant registers to save
} dynsched_task_desc_t;

/// @brief Task
/// @note Used to represent a task
typedef struct {
    dynsched_task_desc_t desc;        // task descriptor
    dynsched_task_context_t context;  // task context
} dynsched_task_t;

/**------------------------------------------------------------------------
 *                           TASK FUNCTIONS
 *------------------------------------------------------------------------**/

/// @brief Create a new task
/// @param mem_manager  The memory manager, used to allocate memory for the task
/// @param task  The task descriptor
dynsched_task_t *dynsched_task_create(dynsched_mem_manager_t *mem_manager, dynsched_task_desc_t *task);

/// @brief Destroy a task
/// @param mem_manager  The memory manager, used to free memory for the task
/// @param task  The task descriptor
void dynsched_task_destroy(dynsched_mem_manager_t *mem_manager, dynsched_task_t *task);

#ifdef __cplusplus
}
#endif

#endif  // __TASK_H__