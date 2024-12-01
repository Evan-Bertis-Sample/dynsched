#ifndef __PREMPT_ESPX_H__
#define __PREMPT_ESPX_H__

#include <driver/timer.h>

#include "dynsched/prempt.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    timer_group_t group_num;
    timer_idx_t timer_num;
    uint32_t (*millis_fn)(void);
} dynsched_prempt_espx_config_t;

typedef enum {
    DYNSCHED_PREMPT_ESPX_STATE_UNINITIALIZED,
    DYNSCHED_PREMPT_ESPX_STATE_IDLE,            // we are not prempting a task, and the hardware timer is not running
    DYNSCHED_PREMPT_ESPX_STATE_RUNNING_PREMPT,  // we are currently prempting a task, meaning the hardware timer is running
    DYNSCHED_PREMPT_ESPX_STATE_STOPPED,         // we have stopped the hardware timer, but we are will not prempt a task
} dynsched_prempt_espx_state_t;

typedef struct {
    uint32_t registers[16];  // the registers
    uint32_t pc;             // the program counter
    uint32_t sp;             // the stack pointer
    uint32_t *stack;         // the stack
    uint32_t stack_size;     // the stack size
} dynsched_prempt_espx_state_buffer_t;

typedef struct {
    uint32_t stack_size;  // the size of the stack
    bool use_epc_reg;     // if true, that means we should use the epc register to restore the program counter
                          // the epc register is used when we are returning from an exception
} dynsched_prempt_espx_state_save_options_t;

typedef struct {
    // configuration
    dynsched_prempt_espx_config_t *platform_config;
    dynsched_mem_manager_t *mem_manager;

    // state variables
    dynsched_prempt_espx_state_t before_prempt_data;  // used to restore the data before the task was prempted
    dynsched_prempt_espx_state_t prempt_task_data;    // used to restore data from the currently running task

    dynsched_prempt_espx_state_t state;
    dynsched_prempt_args_t *last_prempt;
    uint32_t last_prempt_time;  // the last time we prempted a task

} dynsched_prempt_espx_context_t;

dynsched_prempt_interface_t *dynsched_prempt_espx_create(dynsched_mem_manager_t *mem_manager, void *config);
void dynsched_prempt_espx_destroy(dynsched_prempt_interface_t *prempt);

void dynsched_prempt_espx_init(void *ctx);
void dynsched_prempt_espx_start(void *ctx);
void dynsched_prempt_espx_stop(void *ctx);
void dynsched_prempt_espx_prempt(void *ctx, dynsched_prempt_args_t *prempt_args);
void dynsched_prempt_espx_lock(void *ctx);
void dynsched_prempt_espx_unlock(void *ctx);

/**------------------------------------------------------------------------
 *                           ASSEMBLY INTERFACE
 * Because we are doing something so low-level, specifically dealing with
 * context switching, we need to write some assembly code to handle the
 * saving and restoring of the context. These functions are defined purely
 * in assembly. Find them in prempt_espx.S
 *------------------------------------------------------------------------**/

#ifdef __cplusplus
extern "C" {
#endif

void __asm_espx_save_task_context(dynsched_prempt_espx_state_buffer_t *state_buf, dynsched_prempt_espx_state_save_options_t *options);
void __asm_espx_restore_task_context(dynsched_prempt_espx_state_buffer_t *state_buf);

#ifdef __cplusplus
}
#endif

// use these functions to call the assembly functions, which is way nicer for debugging
inline void dynsched_prempt_espx_save_task_context(dynsched_prempt_espx_state_buffer_t *state_buf, dynsched_prempt_espx_state_save_options_t options);
inline void dynsched_prempt_espx_restore_task_context(dynsched_prempt_espx_state_buffer_t *state_buf);

#define DYNSCHED_PREMPT_ESPX                     \
    (dynsched_prempt_interface_t) {              \
        .platform_ctx = NULL,                    \
        .create = dynsched_prempt_espx_create,   \
        .destroy = dynsched_prempt_espx_destroy, \
        .init = dynsched_prempt_espx_init,       \
        .start = dynsched_prempt_espx_start,     \
        .stop = dynsched_prempt_espx_stop,       \
        .prempt = dynsched_prempt_espx_prempt,   \
        .lock = dynsched_prempt_espx_lock,       \
        .unlock = dynsched_prempt_espx_unlock,   \
    }

#ifdef __cplusplus
}
#endif

#endif  // __PREMPT_ESPX_H__