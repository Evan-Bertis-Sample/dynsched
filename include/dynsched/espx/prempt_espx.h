#ifndef __PREMPT_ESPX_H__
#define __PREMPT_ESPX_H__

#include <dynsched/prempt.h>
#include <driver/timer.h>

typedef struct {
    // something something...
    timer_group_t group_num;
    timer_idx_t timer_num;
} dynsched_prempt_espx_config_t;

typedef enum {
    DYNSCHED_PREMPT_ESPX_STATE_IDLE,
    DYNSCHED_PREMPT_ESPX_STATE_RUNNING,
    DYNSCHED_PREMPT_ESPX_STATE_STOPPED,
} dynsched_prempt_espx_state_t;

typedef struct {
    // configuration
    dynsched_prempt_espx_config_t *platform_config;
    dynsched_mem_manager_t *mem_manager;

    // state variables
    dynsched_prempt_espx_state_t state;
    uint32_t last_prempt_time; // the last time we prempted a task

} dynsched_prempt_espx_context_t;

dynsched_prempt_interface_t *dynsched_prempt_espx_create(dynsched_mem_manager_t *mem_manager, void *config);
void dynsched_prempt_espx_destroy(dynsched_prempt_interface_t *prempt);

void dynsched_prempt_espx_init(void *ctx);
void dynsched_prempt_espx_start(void *ctx);
void dynsched_prempt_espx_stop(void *ctx);
void dynsched_prempt_espx_prempt(void *ctx, dynsched_prempt_args_t *args);
void dynsched_prempt_espx_lock(void *ctx);
void dynsched_prempt_espx_unlock(void *ctx);

/**------------------------------------------------------------------------
 *                           ASSEMBLY INTERFACE
 * Because we are doing something so low-level, particularly dealing with
 * context switching, we need to write some assembly code to handle the
 * saving and restoring of the context. These functions are defined purely 
 * in assembly. Find them in prempt_espx.S
 *------------------------------------------------------------------------**/

#ifdef __cplusplus
extern "C" {
#endif

void __asm_espx_save_task_context(dynsched_prempt_interface_t *prempt);
void __asm_espx_restore_task_context(dynsched_prempt_interface_t *prempt);

#ifdef __cplusplus
}
#endif

// use these functions to call the assembly functions, which is way nicer for debugging
void dynsched_prempt_espx_save_task_context(dynsched_prempt_interface_t *prempt);
void dynsched_prempt_espx_restore_task_context(dynsched_prempt_interface_t *prempt);





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