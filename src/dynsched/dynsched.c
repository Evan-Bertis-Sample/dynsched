/**------------------------------------------------------------------------
 *                             dynsched.c
 *
 *  An implementation of a priority scheduler for tasks.
 *
 *
 *------------------------------------------------------------------------**/

#include <driver/timer.h>
#include "dynsched/dynsched.h"

#define DYNSCHED_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)

/**------------------------------------------------------------------------
 *                           PRIORITY SCHEDULER CONSTRUCTOR/DESTRUCTOR
 *------------------------------------------------------------------------**/

dynsched_interface_t *dynsched_psched_create(dynsched_mem_manager_t *mem_manager, void *config) {
    DYNSCHED_PRINT("Creating priority scheduler\n");
    dynsched_psched_context_t *ctx = DYNSCHED_MALLOC(mem_manager, dynsched_psched_context_t);
    dynsched_interface_t *sched = DYNSCHED_MALLOC(mem_manager, dynsched_interface_t);
    ctx->mem_manager = mem_manager;
    ctx->config = *((dynsched_psched_config_t *)config);

    // copy over the template defined in dynsched_psched.h
    memccpy(sched, &dynsched_psched, sizeof(dynsched_interface_t), sizeof(dynsched_interface_t));
    sched->sched_ctx = ctx;
    return sched;
}

void dynsched_psched_destroy(dynsched_interface_t *sched) {
    DYNSCHED_PRINT("Destroying priority scheduler\n");

    dynsched_psched_context_t *ctx = (dynsched_psched_context_t *)sched->sched_ctx;
    // dynsched_pqueue_destroy(ctx->queue);
    dynsched_mem_free(ctx->mem_manager, ctx);
}

/**------------------------------------------------------------------------
 *                           PRIORITY SCHEDULER FUNCTIONS
 *------------------------------------------------------------------------**/

void dynsched_psched_init(void *ctx) {
    DYNSCHED_PRINT("Initializing priority scheduler\n");
    dynsched_psched_context_t *psched_ctx = (dynsched_psched_context_t *)ctx;
    // dynsched_pqueue_t *queue = psched_ctx->queue;
    // dynsched_pqueue_destroy(queue);
    // psched_ctx->queue = dynsched_pqueue_create(psched_ctx->mem_manager, psched_ctx->config.num_priority_levels);
}

void dynsched_psched_add_task(void *ctx, dynsched_task_desc_t *task_desc) {
    DYNSCHED_PRINT("Adding task to priority scheduler\n");
    dynsched_psched_context_t *psched_ctx = (dynsched_psched_context_t *)ctx;

    dynsched_task_t *task = dynsched_task_create(psched_ctx->mem_manager, task_desc);

    // now we need to create the task context
    dynsched_psched_task_context_t *task_ctx = DYNSCHED_MALLOC(psched_ctx->mem_manager, dynsched_psched_task_context_t);

    *task_ctx = (dynsched_psched_task_context_t) {
        .start_time = psched_ctx->miilis_fn(),
        .priority = 0,
        .task_registers = dynsched_mem_alloc(psched_ctx->mem_manager, task->desc.register_size),
        .task_stack = dynsched_mem_alloc(psched_ctx->mem_manager, task->desc.stack_size),
    };

    // we need to add the task to our prority queue
    // dynsched_pqueue_insert(psched_ctx->queue, task, task_ctx->priority);

}

void dynsched_psched_remove_task(void *ctx, char *task_name) {
    DYNSCHED_PRINT("Removing task from priority scheduler\n");
    // dynsched_psched_context_t *psched_ctx = (dynsched_psched_context_t *)ctx;
    // dynsched_pqueue_node_t *node = dynsched_pqueue_peek_node(psched_ctx->queue);
    // while (node != NULL) {
    //     dynsched_task_desc_t *task = (dynsched_task_desc_t *)node->data;
    //     if (strcmp(task->task_name, task_name) == 0) {
    //         dynsched_pqueue_remove(psched_ctx->queue, node);
    //         dynsched_pqueue_node_destroy(node);
    //         return;
    //     }
    //     node = dynsched_pqueue_next(psched_ctx->queue);
    // }


}

void dynsched_psched_run(void *ctx) {
    DYNSCHED_PRINT("Running priority scheduler\n");
    dynsched_psched_context_t *psched_ctx = (dynsched_psched_context_t *)ctx;
    // dynsched_pqueue_t *queue = psched_ctx->queue;
    // dynsched_pqueue_begin(queue);
    // dynsched_pqueue_node_t *node = dynsched_pqueue_peek_node(queue);
    // while (node != NULL) {
    //     dynsched_task_desc_t *task = (dynsched_task_desc_t *)node->data;
    //     task->task_func(task->task_ctx);
    //     node = dynsched_pqueue_next(queue);
    // }
}

/**------------------------------------------------------------------------
 *                           PRIORITY SCHEDULER ITERATOR FUNCTIONS
 *------------------------------------------------------------------------**/

dynsched_task_t *dynsched_psched_begin(void *ctx) {
    DYNSCHED_PRINT("Getting first task from priority scheduler\n");
    dynsched_psched_context_t *psched_ctx = (dynsched_psched_context_t *)ctx;
    // dynsched_pqueue_begin(psched_ctx->queue);
    // dynsched_pqueue_node_t *node = dynsched_pqueue_peek_node(psched_ctx->queue);
    // if (node == NULL) {
    //     return NULL;
    // }
    // dynsched_task_desc_t *task = (dynsched_task_desc_t *)node->data;
    // dynsched_task_t *task_ctx = (dynsched_task_t *)dynsched_mem_alloc(psched_ctx->mem_manager, sizeof(dynsched_task_t));
    // task_ctx->task = *task;
    // task_ctx->context.sched_task_ctx = ctx;
    // task_ctx->context.task_ctx = task->task_ctx;
    // return task_ctx;
}

dynsched_task_t *dynsched_psched_next(void *ctx) {
    DYNSCHED_PRINT("Getting next task from priority scheduler\n");
    dynsched_psched_context_t *psched_ctx = (dynsched_psched_context_t *)ctx;
    // dynsched_pqueue_node_t *node = dynsched_pqueue_next(psched_ctx->queue);
    // if (node == NULL) {
    //     return NULL;
    // }
    // dynsched_task_desc_t *task = (dynsched_task_desc_t *)node->data;
    // dynsched_task_t *task_ctx = (dynsched_task_t *)dynsched_mem_alloc(psched_ctx->mem_manager, sizeof(dynsched_task_t));
    // task_ctx->task = *task;
    // task_ctx->context.sched_task_ctx = ctx;
    // task_ctx->context.task_ctx = task->task_ctx;
    // return task_ctx;
}

dynsched_task_t *dynsched_psched_end(void *ctx) {
    DYNSCHED_PRINT("Getting last task from priority scheduler\n");
    return NULL;
}
