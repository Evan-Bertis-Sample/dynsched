/**------------------------------------------------------------------------
 *                             prempt_espx.c
 *
 *  Hardware-specific implementation of the preemption interface for the ESP32.
 *  Opertates on the ESP32's FreeRTOS implementation.
 *
 *------------------------------------------------------------------------**/

#include "dynsched/espx/prempt_espx.h"

#include <freertos/FreeRTOS.h>
#include <freertos/mpu_wrappers.h>
#include <freertos/task.h>
#include <freertos/xtensa_rtos.h>

#define DYNSCHED_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)

/**------------------------------------------------------------------------
 *                           ESP32 PREMPTION HELPERS
 *------------------------------------------------------------------------**/

void dynsched_prempt_espx_save_task_context(dynsched_prempt_espx_state_buffer_t *state_buf, dynsched_prempt_espx_state_save_options_t options) {
    DYNSCHED_PRINT("Saving ESP32 task context\n");
    // disable interrupts, so we can save the context
    // portDISABLE_INTERRUPTS();
    // for now, just to check for errors, write some random data to the stack
    // memset(state_buf->stack, 0x55, state_buf->stack_size);
    DYNSCHED_PRINT("Assembly Instruction address: %p\n", __asm_espx_save_task_context);
    __asm_espx_save_task_context(state_buf, &options);

    DYNSCHED_PRINT("Saved ESP32 task context\n");
    DYNSCHED_PRINT("Stack size: %d\n", state_buf->stack_size);
    // portENABLE_INTERRUPTS();
}

void dynsched_prempt_espx_restore_task_context(dynsched_prempt_espx_state_buffer_t *state_buf) {
    DYNSCHED_PRINT("Restoring ESP32 task context\n");
    // disable interrupts, so we can restore the context
    portDISABLE_INTERRUPTS();
    __asm_espx_restore_task_context(state_buf);
    portENABLE_INTERRUPTS();
}

bool espx_timer_isr_handler(void *args) {
    // we should have a seperate stack for the ISR
    // this isr is called after a hardware timer has expired
    // for our purposes, that means a task has been prempted
    DYNSCHED_PRINT("Handling ESP32 timer ISR\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)args;
    // this is called after a task that was prempted has finished, either by timeout or the task finishing
    uint32_t current_time = prempt_ctx->platform_config->millis_fn();
    bool prempted = (current_time - prempt_ctx->last_prempt_time) >= prempt_ctx->last_prempt->prempt_time;

    if (prempted) {
        // we should store the current task's context, so we can restore it later
        dynsched_prempt_espx_state_save_options_t options = {
            .stack_size = prempt_ctx->last_prempt->stack_size,  // save using the old stack size
            .use_epc_reg = true                                 // we are returning from an exception, so we should use the epc register
        };
        dynsched_prempt_espx_save_task_context(prempt_ctx->prempt_task_data, options);
        prempt_ctx->state = DYNSCHED_PREMPT_ESPX_STATE_IDLE;
    }

    // now we should restore the old context, which was saved before the task was prempted
    dynsched_prempt_espx_restore_task_context(prempt_ctx->before_prempt_data);
}

dynsched_prempt_espx_state_buffer_t *dynsched_prempt_espx_create_state_buffer(dynsched_mem_manager_t *mem_manager, uint32_t stack_size) {
    DYNSCHED_PRINT("Creating ESP32 state buffer\n");
    dynsched_prempt_espx_state_buffer_t *state_buf = DYNSCHED_MALLOC(mem_manager, dynsched_prempt_espx_state_buffer_t);
    state_buf->stack_size = stack_size;
    state_buf->stack = dynsched_mem_alloc(mem_manager, stack_size);
    return state_buf;
}

void dynsched_prempt_espx_state_buffer_resize(dynsched_mem_manager_t *mem_manager, dynsched_prempt_espx_state_buffer_t *state_buf, uint32_t new_stack_size) {
    DYNSCHED_PRINT("Resizing ESP32 state buffer\n");
    dynsched_mem_free(mem_manager, state_buf->stack);
    state_buf->stack_size = new_stack_size;
    state_buf->stack = dynsched_mem_alloc(mem_manager, new_stack_size);
}

void dynsched_prempt_espx_destroy_state_buffer(dynsched_mem_manager_t *mem_manager, dynsched_prempt_espx_state_buffer_t *state_buf) {
    DYNSCHED_PRINT("Destroying ESP32 state buffer\n");
    dynsched_mem_free(mem_manager, state_buf->stack);
    dynsched_mem_free(mem_manager, state_buf);
}

/**------------------------------------------------------------------------
 *                           PREMPTION INTERFACE CONSTRUCTOR/DESTRUCTOR
 *------------------------------------------------------------------------**/

dynsched_prempt_interface_t *dynsched_prempt_espx_create(dynsched_mem_manager_t *mem_manager, void *config) {
    DYNSCHED_PRINT("Creating ESP32 preemption interface\n");

    dynsched_prempt_espx_context_t *ctx = DYNSCHED_MALLOC(mem_manager, dynsched_prempt_espx_context_t);
    dynsched_prempt_interface_t *prempt = DYNSCHED_MALLOC(mem_manager, dynsched_prempt_interface_t);
    ctx->platform_config = config;

    // copy over the template defined in dynsched_prempt_espx.h
    *prempt = DYNSCHED_PREMPT_ESPX;
    prempt->platform_ctx = ctx;

    // create the state buffers
    ctx->before_prempt_data = dynsched_prempt_espx_create_state_buffer(mem_manager, 64);
    ctx->prempt_task_data = dynsched_prempt_espx_create_state_buffer(mem_manager, 64);

    return prempt;
}

void dynsched_prempt_espx_destroy(dynsched_prempt_interface_t *prempt) {
    DYNSCHED_PRINT("Destroying ESP32 preemption interface\n");
    dynsched_prempt_espx_context_t *ctx = (dynsched_prempt_espx_context_t *)prempt->platform_ctx;
    dynsched_mem_free(ctx->mem_manager, ctx);
}

/**------------------------------------------------------------------------
 *                           PREMPTION INTERFACE FUNCTIONS
 *------------------------------------------------------------------------**/

void dynsched_prempt_espx_init(void *ctx) {
    DYNSCHED_PRINT("Initializing ESP32 preemption interface\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;

    // initialize the hardware timer, which will be used to prempt tasks
    timer_config_t timer_config = {
        .alarm_en = TIMER_ALARM_EN,
        .counter_en = TIMER_PAUSE,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = TIMER_AUTORELOAD_EN,
        .divider = 80  // 1us per tick
    };

    timer_init(prempt_ctx->platform_config->group_num,
               prempt_ctx->platform_config->timer_num,
               &timer_config);

    prempt_ctx->state = DYNSCHED_PREMPT_ESPX_STATE_IDLE;

}

void dynsched_prempt_espx_start(void *ctx) {
    DYNSCHED_PRINT("Starting ESP32 preemption interface\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;

    if (prempt_ctx->state != DYNSCHED_PREMPT_ESPX_STATE_IDLE)
    {
        DYNSCHED_PRINT("ERROR: Starting ESP32 preemption interface in an invalid state!\n");
    }            
}

void dynsched_prempt_espx_stop(void *ctx) {
    DYNSCHED_PRINT("Stopping ESP32 preemption interface\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
}

void dynsched_prempt_espx_prempt(void *ctx, dynsched_prempt_args_t *prempt_args) {
    DYNSCHED_PRINT("Preempting ESP32 preemption interface\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;

    // this function should only ever be called from the scheduler, when we
    // are not in the middle of a task
    // if we are in the middle of a task, something has seriously gone wrong
    if (prempt_ctx->state != DYNSCHED_PREMPT_ESPX_STATE_IDLE) {
        DYNSCHED_PRINT("ERROR: Attempting to prempt a task while in the middle of another task\n");
        return;
    }

    // set up the hardware timer to prempt the task
    timer_set_counter_value(prempt_ctx->platform_config->group_num,
                            prempt_ctx->platform_config->timer_num,
                            0);

    timer_set_alarm_value(prempt_ctx->platform_config->group_num,
                          prempt_ctx->platform_config->timer_num,
                          prempt_args->prempt_time);

    timer_set_alarm(prempt_ctx->platform_config->group_num,
                    prempt_ctx->platform_config->timer_num,
                    TIMER_ALARM_EN);

    prempt_ctx->last_prempt = prempt_args;
    prempt_ctx->last_prempt_time = prempt_ctx->platform_config->millis_fn();
    prempt_ctx->state = DYNSCHED_PREMPT_ESPX_STATE_RUNNING_PREMPT;

    // now we should save the current world's state, so we can restore it later
    dynsched_prempt_espx_state_save_options_t options = {
        .stack_size = prempt_args->stack_size,
        .use_epc_reg = false  // we are not returning from an exception, so we should not use the epc register
    };

    dynsched_prempt_espx_save_task_context(prempt_ctx->before_prempt_data, options);

    // now we will run the function
    prempt_args->prempt_func(prempt_args->task_data);

    // if we return in time, then we should stop the timer
    uint32_t time = prempt_ctx->platform_config->millis_fn();
    int32_t time_elapsed = (int32_t)time - (int32_t)prempt_ctx->last_prempt_time;

    if (time_elapsed < (int32_t)prempt_ctx->last_prempt->prempt_time) {
        // we know we actually didn't need to preempt this
        // therefore, we should stop the timer
        DYNSCHED_PRINT("Preemptive Task ran to completion!");
        timer_pause(prempt_ctx->platform_config->group_num, prempt_ctx->platform_config->timer_num);
        prempt_ctx->state = DYNSCHED_PREMPT_ESPX_STATE_IDLE;
    }
}

void dynsched_prempt_espx_lock(void *ctx) {
    DYNSCHED_PRINT("Locking ESP32 preemption interface\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
}

void dynsched_prempt_espx_unlock(void *ctx) {
    DYNSCHED_PRINT("Unlocking ESP32 preemption interface\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
}
