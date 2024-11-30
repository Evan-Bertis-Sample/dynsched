/*------------------------------------------------------------------------
 * prempt_espx.S
 *
 * Assembly functions for saving and restoring task context in Xtensa architecture.
 * These functions handle low-level context switching by saving and restoring
 * general-purpose registers and the stack pointer.
 *
 *------------------------------------------------------------------------*/

    .section .text
    .align 4

    /*------------------------------------------------------------------------
     * Function: __asm_espx_save_task_context
     * Saves the current task context into the buffers provided in prempt_args.
     *
     * Arguments:
     *   a2 - Pointer to dynsched_prempt_args_t structure
     *------------------------------------------------------------------------*/
    .global __asm_espx_save_task_context
__asm_espx_save_task_context:
    /* Load the address of register_buffer from prempt_args */
    l32i   a3, a2, 0          /* a3 = prempt_args->register_buffer */
    mov    a5, a3             /* a5 = register_buffer (temporary base pointer) */

    /* Save general-purpose registers to register_buffer */
    s32i   a0, a5, 0          /* Save a0 */
    s32i   a1, a5, 4          /* Save a1 (stack pointer) */
    s32i   a2, a5, 8          /* Save a2 */
    s32i   a3, a5, 12         /* Save a3 */
    s32i   a4, a5, 16         /* Save a4 */
    s32i   a5, a5, 20         /* Save a5 */
    s32i   a6, a5, 24         /* Save a6 */
    s32i   a7, a5, 28         /* Save a7 */
    s32i   a8, a5, 32         /* Save a8 */
    s32i   a9, a5, 36         /* Save a9 */
    s32i   a10, a5, 40        /* Save a10 */
    s32i   a11, a5, 44        /* Save a11 */
    s32i   a12, a5, 48        /* Save a12 */
    s32i   a13, a5, 52        /* Save a13 */
    s32i   a14, a5, 56        /* Save a14 */
    s32i   a15, a5, 60        /* Save a15 */

    /* Save the current stack pointer to stack_buffer */
    l32i   a4, a2, 4          /* a4 = prempt_args->stack_buffer */
    s32i   a1, a4, 0          /* Save stack pointer (a1) */

    /* Return from the function */
    ret

    /*------------------------------------------------------------------------
     * Function: __asm_espx_restore_task_context
     * Restores the task context from the buffers provided in prempt_args.
     *
     * Arguments:
     *   a2 - Pointer to dynsched_prempt_args_t structure
     *------------------------------------------------------------------------*/
    .global __asm_espx_restore_task_context
__asm_espx_restore_task_context:
    /* Load the address of register_buffer from prempt_args */
    l32i   a3, a2, 0          /* a3 = prempt_args->register_buffer */

    /* Temporarily store the saved return address (a0) */
    l32i   a5, a3, 0          /* a5 = saved a0 (return address) */

    /* Restore general-purpose registers except a1 (stack pointer) */
    l32i   a2, a3, 8          /* Restore a2 */
    l32i   a4, a3, 16         /* Restore a4 */
    l32i   a6, a3, 24         /* Restore a6 */
    l32i   a7, a3, 28         /* Restore a7 */
    l32i   a8, a3, 32         /* Restore a8 */
    l32i   a9, a3, 36         /* Restore a9 */
    l32i   a10, a3, 40        /* Restore a10 */
    l32i   a11, a3, 44        /* Restore a11 */
    l32i   a12, a3, 48        /* Restore a12 */
    l32i   a13, a3, 52        /* Restore a13 */
    l32i   a14, a3, 56        /* Restore a14 */
    l32i   a15, a3, 60        /* Restore a15 */

    /* Restore the stack pointer */
    l32i   a1, a3, 4          /* Restore a1 (stack pointer) */

    /* Restore a3 after using it */
    l32i   a3, a3, 12         /* Restore a3 */

    /* Restore the return address */
    mov    a0, a5             /* a0 = saved return address */

    /* Return from the function */
    ret
