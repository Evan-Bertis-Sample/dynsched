/**------------------------------------------------------------------------
 *                             mem.c
 *
 *  Memory manager implementation.
 *
 *
 *------------------------------------------------------------------------**/

#include "dynsched/mem_manager.h"

void dynsched_mem_init(dynsched_mem_manager_t *manager, void *ctx) { manager->init(ctx); }

void *dynsched_mem_alloc(dynsched_mem_manager_t *manager, dynsched_mem_size_t size) {
    return manager->alloc(manager->manager_ctx, size);
}

void dynsched_mem_free(dynsched_mem_manager_t *manager, void *ptr) {
    manager->free(manager->manager_ctx, ptr);
}

#ifndef DYNSCHED_DEFAULT_MEM_UNIMPLEMENTED

void dynsched_default_mem_init(void *ctx) { return; }
void *dynsched_default_mem_alloc(void *ctx, dynsched_mem_size_t size) { return malloc(size); }
void dynsched_default_mem_free(void *ctx, void *ptr) { free(ptr); }

#endif  // !DYNSCHED_DEFAULT_MEM_UNIMPLEMENTED