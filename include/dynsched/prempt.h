#ifndef __PREMPT_H__
#define __PREMPT_H__

#include "dynsched/mem_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

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

inline dynsched_prempt_interface_t *dynsched_prempt_create(dynsched_mem_manager_t *mem_manager, dynsched_prempt_interface_t interface, void *config);
inline void dynsched_prempt_destroy(dynsched_prempt_interface_t *prempt);
inline void dynsched_prempt_init(dynsched_prempt_interface_t *prempt);
inline void dynsched_prempt_start(dynsched_prempt_interface_t *prempt);
inline void dynsched_prempt_stop(dynsched_prempt_interface_t *prempt);
inline void dynsched_prempt_prempt(dynsched_prempt_interface_t *prempt, dynsched_prempt_args_t *args);
inline void dynsched_prempt_lock(dynsched_prempt_interface_t *prempt);
inline void dynsched_prempt_unlock(dynsched_prempt_interface_t *prempt);

#ifdef __cplusplus
}
#endif

#endif  // __PREMPT_H__