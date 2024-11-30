#ifndef __MEM_H__
#define __MEM_H__
#include <stddef.h>
#include <stdint.h>

/**------------------------------------------------------------------------
 *                           MEMORY MANAGER STRUCTURES
 *------------------------------------------------------------------------**/

typedef size_t dynsched_mem_size_t;

typedef struct {
    void *manager_ctx;                                    // holds the memory manager context, for internal use
    void (*init)(void *ctx);                              // initialize the memory manager
    void *(*alloc)(void *ctx, dynsched_mem_size_t size);  // allocate memory
    void (*free)(void *ctx, void *ptr);                   // free memory
} dynsched_mem_manager_t;

/**------------------------------------------------------------------------
 *                           MEMORY MANAGER FUNCTIONS
 *------------------------------------------------------------------------**/

void dynsched_mem_init(dynsched_mem_manager_t *manager);
void *dynsched_mem_alloc(dynsched_mem_manager_t *manager, dynsched_mem_size_t size);
void dynsched_mem_free(dynsched_mem_manager_t *manager, void *ptr);

/**------------------------------------------------------------------------
 *             MEMORY MANAGER DEFAULT IMPLEMENTATION (MALLOC/FREE)
 *------------------------------------------------------------------------**/

// create a default implementation for malloc & free, if defined on this system
// by default, we are going to assume that malloc and free are defined
// if not, define DYNSCHED_DEFAULT_MEM_UNIMPLEMENTED
#ifndef DYNSCHED_DEFAULT_MEM_UNIMPLEMENTED
#include <memory.h>

void dynsched_default_mem_init(void *ctx) { return; }
void *dynsched_default_mem_alloc(void *ctx, dynsched_mem_size_t size) { return malloc(size); }
void dynsched_default_mem_free(void *ctx, void *ptr) { free(ptr); }

dynsched_mem_manager_t dynsched_default_mem_manager =

#define DYNSCHED_DEFAULT_MEM                 \
    (dynsched_mem_manager_t) {               \
        .manager_ctx = NULL,                 \
        .init = dynsched_default_mem_init,   \
        .alloc = dynsched_default_mem_alloc, \
        .free = dynsched_default_mem_free    \
    }

#define DYNSCHED_MALLOC(manager, type) (type *)dynsched_mem_alloc(manager, sizeof(type))

#endif

#endif  // __MEM_H__