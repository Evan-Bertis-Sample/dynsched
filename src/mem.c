/**------------------------------------------------------------------------
 *                             mem.c
 *
 *  Memory manager implementation.
 *
 *
 *------------------------------------------------------------------------**/

#include <dynsched/mem.h>

void dynsched_mem_init(dynsched_mem_manager_t *manager, void *ctx) { manager->init(ctx); }


void *dynsched_mem_alloc(dynsched_mem_manager_t *manager, dynsched_mem_size_t size) {
    return manager->alloc(manager->manager_ctx, size);
}


void dynsched_mem_free(dynsched_mem_manager_t *manager, void *ptr) {
    manager->free(manager->manager_ctx, ptr);
}