/**------------------------------------------------------------------------
 *                             prempt_espx.c
 *
 *  Hardware-specific implementation of the preemption interface for the ESP32.
 *  Opertates on the ESP32's FreeRTOS implementation.
 *
 *------------------------------------------------------------------------**/

#include <dynsched/prempt_espx.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define DYNSCHED_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)


/**------------------------------------------------------------------------
 *                           PREMPTION INTERFACE CONSTRUCTOR/DESTRUCTOR
 *------------------------------------------------------------------------**/

dynsched_prempt_interface_t *dynsched_prempt_espx_create(dynsched_mem_manager_t *mem_manager, void *config) {
    DYNSCHED_PRINT("Creating ESP32 preemption interface\n");

    dynsched_prempt_espx_context_t *ctx = (dynsched_prempt_espx_context_t *)dynsched_mem_alloc(mem_manager, sizeof(dynsched_prempt_espx_context_t));
    dynsched_prempt_interface_t *prempt = (dynsched_prempt_interface_t *)dynsched_mem_alloc(mem_manager, sizeof(dynsched_prempt_interface_t));
    ctx->platform_ctx = config;

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
    // dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
}

void dynsched_prempt_espx_start(void *ctx) {
    DYNSCHED_PRINT("Starting ESP32 preemption interface\n");
    // dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
}

void dynsched_prempt_espx_stop(void *ctx) {
    DYNSCHED_PRINT("Stopping ESP32 preemption interface\n");
    // dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
}

void dynsched_prempt_espx_prempt(void *ctx, dynsched_prempt_args_t *args) {
    DYNSCHED_PRINT("Preempting ESP32 preemption interface\n");
    // dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
    // args->prempt_func(args->platform_ctx);
}

void dynsched_prempt_espx_lock(void *ctx) {
    DYNSCHED_PRINT("Locking ESP32 preemption interface\n");
    // dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
}

void dynsched_prempt_espx_unlock(void *ctx) {
    DYNSCHED_PRINT("Unlocking ESP32 preemption interface\n");
    // dynsched_prempt_espx_context_t *prempt_ctx = (dynsched_prempt_espx_context_t *)ctx;
}

