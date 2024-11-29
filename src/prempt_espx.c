/**------------------------------------------------------------------------
 *                             prempt_espx.c
 *
 *  Hardware-specific implementation of the preemption interface for the ESP32.
 *  Opertates on the ESP32's FreeRTOS implementation.
 *
 *------------------------------------------------------------------------**/

#include <dynsched/espx/prempt_espx.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define DYNSCHED_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)

/**------------------------------------------------------------------------
 *                           ESP32 PREMPTION HELPERS
 *------------------------------------------------------------------------**/

bool espx_timer_isr_handler(void *args) {
    DYNSCHED_PRINT("Handling ESP32 timer ISR\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)args;
}

/**------------------------------------------------------------------------
 *                           PREMPTION INTERFACE CONSTRUCTOR/DESTRUCTOR
 *------------------------------------------------------------------------**/

dynsched_prempt_interface_t *dynsched_prempt_espx_create(dynsched_mem_manager_t *mem_manager, void *config) {
    DYNSCHED_PRINT("Creating ESP32 preemption interface\n");

    dynsched_prempt_espx_context_t *ctx = (dynsched_prempt_espx_context_t *)dynsched_mem_alloc(mem_manager, sizeof(dynsched_prempt_espx_context_t));
    dynsched_prempt_interface_t *prempt = (dynsched_prempt_interface_t *)dynsched_mem_alloc(mem_manager, sizeof(dynsched_prempt_interface_t));
    ctx->platform_config = config;

    // copy over the template defined in dynsched_prempt_espx.h
    memccpy(prempt, &dynsched_prempt_espx, sizeof(dynsched_prempt_interface_t), sizeof(dynsched_prempt_interface_t));
    prempt->platform_ctx = ctx;
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
}

void dynsched_prempt_espx_start(void *ctx) {
    DYNSCHED_PRINT("Starting ESP32 preemption interface\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
}

void dynsched_prempt_espx_stop(void *ctx) {
    DYNSCHED_PRINT("Stopping ESP32 preemption interface\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
}

void dynsched_prempt_espx_prempt(void *ctx, dynsched_prempt_args_t *args) {
    DYNSCHED_PRINT("Preempting ESP32 preemption interface\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;

    // this function should only ever be called from the scheduler, when we 
    // are not in the middle of a task
    // if we are in the middle of a task, something has seriously gone wrong          
}

void dynsched_prempt_espx_lock(void *ctx) {
    DYNSCHED_PRINT("Locking ESP32 preemption interface\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
}

void dynsched_prempt_espx_unlock(void *ctx) {
    DYNSCHED_PRINT("Unlocking ESP32 preemption interface\n");
    dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
}
