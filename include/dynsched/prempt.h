#ifndef __PREMPT_H__
#define __PREMPT_H__

#include "dynsched/mem_manager.h"

typedef struct {
    void *task_data;  // used to call the task's function
    uint32_t prempt_time;
    uint32_t stack_size;  // the size of the stack to use for the task
    void (*prempt_func)(void *);
    void (*after_prempt_func)(void *);
} dynsched_prempt_args_t;

typedef struct dynsched_prempt_interface {
    void *platform_ctx;

    // constructor/destructor
    struct dynsched_prempt_interface *(*create)(dynsched_mem_manager_t *mem_manager, void *config);
    void (*destroy)(struct dynsched_prempt_interface *prempt);

    // prempt functions
    void (*init)(void *ctx);
    void (*start)(void *ctx);
    void (*stop)(void *ctx);
    void (*prempt)(void *ctx, dynsched_prempt_args_t *args);

    // lock functions
    void (*lock)(void *ctx);
    void (*unlock)(void *ctx);

} dynsched_prempt_interface_t;

dynsched_prempt_interface_t *dynsched_prempt_create(dynsched_mem_manager_t *mem_manager, dynsched_prempt_interface_t interface, void *config) {
    dynsched_prempt_interface_t *instance = interface.create(mem_manager, config);
    return instance;
}

void dynsched_prempt_destroy(dynsched_prempt_interface_t *prempt) { prempt->destroy(prempt); }
void dynsched_prempt_init(dynsched_prempt_interface_t *prempt) { prempt->init(prempt->platform_ctx); }
void dynsched_prempt_start(dynsched_prempt_interface_t *prempt) { prempt->start(prempt->platform_ctx); }
void dynsched_prempt_stop(dynsched_prempt_interface_t *prempt) { prempt->stop(prempt->platform_ctx); }
void dynsched_prempt_prempt(dynsched_prempt_interface_t *prempt, dynsched_prempt_args_t *args) { prempt->prempt(prempt->platform_ctx, args); }
void dynsched_prempt_lock(dynsched_prempt_interface_t *prempt) { prempt->lock(prempt->platform_ctx); }
void dynsched_prempt_unlock(dynsched_prempt_interface_t *prempt) { prempt->unlock(prempt->platform_ctx); }

#endif  // __PREMPT_H__