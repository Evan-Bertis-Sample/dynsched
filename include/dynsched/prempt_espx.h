#ifndef __PREMPT_ESPX_H__
#define __PREMPT_ESPX_H__

#include <dynsched/prempt.h>

typedef struct {
    // something something...
} dynsched_prempt_espx_config_t;

typedef struct {
    // something something...
} dynsched_prempt_espx_context_t;

dynsched_prempt_interface_t *dynsched_prempt_espx_create(dynsched_mem_manager_t *mem_manager, void *config);
void dynsched_prempt_espx_destroy(dynsched_prempt_interface_t *prempt);

void dynsched_prempt_espx_init(void *ctx);
void dynsched_prempt_espx_start(void *ctx);
void dynsched_prempt_espx_stop(void *ctx);
void dynsched_prempt_espx_prempt(void *ctx, dynsched_prempt_args_t *args);
void dynsched_prempt_espx_lock(void *ctx);
void dynsched_prempt_espx_unlock(void *ctx);

dynsched_prempt_interface_t dynsched_prempt_espx = {
    .platform_ctx = NULL,

    // constructor/destructor
    .create = dynsched_prempt_espx_create,
    .destroy = dynsched_prempt_espx_destroy,

    // prempt functions
    .init = dynsched_prempt_espx_init,
    .start = dynsched_prempt_espx_start,
    .stop = dynsched_prempt_espx_stop,
    .prempt = dynsched_prempt_espx_prempt,

    // lock functions
    .lock = dynsched_prempt_espx_lock,
    .unlock = dynsched_prempt_espx_unlock,
};

#endif  // __PREMPT_ESPX_H__