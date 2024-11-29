#ifndef __preempt_H__
#define __preempt_H__

#include <dynsched/mem.h>

typedef struct {
    uint32_t preempt_time;
    void (*preempt_func)(void *);
    void (*after_preempt_func)(void *);
} dynsched_preempt_args_t;

typedef struct {
    void *platform_ctx;

    // constructor/destructor
    dynsched_preempt_interface_t *(*create)(dynsched_mem_manager_t *mem_manager, void *config);
    void (*destroy)(dynsched_preempt_interface_t *preempt);

    // preempt functions
    void (*init)(void *ctx);
    void (*start)(void *ctx);
    void (*stop)(void *ctx);
    void (*preempt)(void *ctx, dynsched_preempt_args_t *args);

    // lock functions
    void (*lock)(void *ctx);
    void (*unlock)(void *ctx);

} dynsched_preempt_interface_t;

dynsched_preempt_interface_t dynsched_preempt_create(dynsched_mem_manager_t *mem_manager, dynsched_preempt_interface_t *interface, void *config) {
    dynsched_preempt_interface_t *interface = interface->create(mem_manager, config);
    return *interface;
}

void dynsched_preempt_destroy(dynsched_preempt_interface_t *preempt) { preempt->destroy(preempt); }
void dynsched_preempt_init(dynsched_preempt_interface_t *preempt) { preempt->init(preempt->platform_ctx); }
void dynsched_preempt_start(dynsched_preempt_interface_t *preempt) { preempt->start(preempt->platform_ctx); }
void dynsched_preempt_stop(dynsched_preempt_interface_t *preempt) { preempt->stop(preempt->platform_ctx); }
void dynsched_preempt_preempt(dynsched_preempt_interface_t *preempt, dynsched_preempt_args_t *args) { preempt->preempt(preempt->platform_ctx, args); }
void dynsched_preempt_lock(dynsched_preempt_interface_t *preempt) { preempt->lock(preempt->platform_ctx); }
void dynsched_preempt_unlock(dynsched_preempt_interface_t *preempt) { preempt->unlock(preempt->platform_ctx); }

#endif  // __preempt_H__