/**------------------------------------------------------------------------
 *                             prempt.c
 *
 *  Implementation of the preemption interface.
 *
 *
 *------------------------------------------------------------------------**/

#include "dynsched/prempt.h"

dynsched_prempt_interface_t *dynsched_prempt_create(dynsched_mem_manager_t *mem_manager, dynsched_prempt_interface_t interface, void *config) {
    dynsched_prempt_interface_t *instance = interface.create(mem_manager, config);
    return instance;
}

void dynsched_prempt_destroy(dynsched_prempt_interface_t *prempt) {
    prempt->destroy(prempt);
}

void dynsched_prempt_init(dynsched_prempt_interface_t *prempt) {
    prempt->init(prempt->platform_ctx);
}

void dynsched_prempt_start(dynsched_prempt_interface_t *prempt) {
    prempt->start(prempt->platform_ctx);
}

void dynsched_prempt_stop(dynsched_prempt_interface_t *prempt) {
    prempt->stop(prempt->platform_ctx);
}

void dynsched_prempt_prempt(dynsched_prempt_interface_t *prempt, dynsched_prempt_args_t *args) {
    prempt->prempt(prempt->platform_ctx, args);
}

void dynsched_prempt_lock(dynsched_prempt_interface_t *prempt) {
    prempt->lock(prempt->platform_ctx);
}

void dynsched_prempt_unlock(dynsched_prempt_interface_t *prempt) {
    prempt->unlock(prempt->platform_ctx);
}
