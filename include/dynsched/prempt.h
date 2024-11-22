#ifndef __PREMPT_H__
#define __PREMPT_H__

#include <esp_attr.h>
#include <esp_intr_alloc.h>
#include <esp_system.h>
#include <soc/rtc.h>

typedef struct {
    uint32_t prempt_time;
    void (*prempt_func)(void *);
    void (*after_prempt_func)(void *);
} dynsched_prempt_args_t;

typedef struct {
    void *platform_ctx;

    // constructor/destructor
    dynsched_prempt_interface_t (*create)(void *platform_ctx);
    void (*destroy)(dynsched_prempt_interface_t *prempt);

    // prempt functions
    void (*init)(void *ctx);
    void (*start)(void *ctx);
    void (*stop)(void *ctx);
    void (*prempt)(void *ctx, dynsched_prempt_args_t *args);

    // lock functions
    void (*lock)(void *ctx);
    void (*unlock)(void *ctx);

} dynsched_prempt_interface_t;


dynsched_prempt_interface_t dynsched_prempt_create(void *platform_ctx);
void dynsched_prempt_destroy(dynsched_prempt_interface_t *prempt);
void dynsched_prempt_init(dynsched_prempt_interface_t *prempt);
void dynsched_prempt_start(dynsched_prempt_interface_t *prempt);
void dynsched_prempt_stop(dynsched_prempt_interface_t *prempt);
void dynsched_prempt_prempt(dynsched_prempt_interface_t *prempt, dynsched_prempt_args_t *args);
void dynsched_prempt_lock(dynsched_prempt_interface_t *prempt);
void dynsched_prempt_unlock(dynsched_prempt_interface_t *prempt);

#endif  // __PREMPT_H__