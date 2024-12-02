/*------------------------------------------------------------------------
 * prempt_espx.S
 *
 * Assembly functions for saving and restoring task context in Xtensa architecture.
 * These functions handle low-level context switching by saving and restoring
 * general-purpose registers, program counter, stack pointer, and stack contents.
 * Interrupts are disabled during these operations to prevent interference.
 *
 *------------------------------------------------------------------------*/

    .file 1 "prempt_espx.S"

    .section .text
    .align 4

    /* Offsets within dynsched_prempt_espx_state_buffer_t */
    /* Assuming standard alignment, the offsets are as follows:
     * registers[0..15]: offsets 0 to 60 (0x00 to 0x3C)
     * pc: offset 64 (0x40)
     * sp: offset 68 (0x44)
     * stack:  offset 72 (0x48)
     * stack_size: offset 76 (0x4C)
     */

    /* Offsets within dynsched_prempt_espx_state_save_options_t */
    /* stack_size: offset 0
     * use_epc_reg: offset 4
     */
    /*------------------------------------------------------------------------
     * Function: __asm_espx_save_task_context
     * Saves the current task context into the state buffer provided.
     * Interrupts are disabled during this operation.
     *
     * Arguments:
     *   a2 - Pointer to dynsched_prempt_espx_state_buffer_t (state_buf)
     *   a3 - Pointer to dynsched_prempt_espx_state_save_options_t (options)
     *------------------------------------------------------------------------*/
    .global __asm_espx_save_task_context
__asm_espx_save_task_context:
    ret
    .loc 1 10 0                  /* Source file: 'prempt_espx.S', line 10 */
    // /* Send debug message */
    // movi   a2, debug_msg_start
    // call0  printf

    /* Disable interrupts and save previous PS */
    // rsil   a4, 15                /* Disable interrupts, save PS to a4 */

    /* Save registers that will be modified */
    .loc 1 20 0                  /* Source file: 'prempt_espx.S', line 20 */
    addi   a1, a1, -16           /* Adjust stack pointer to save 4 registers */
    s32i   a5, a1, 0             /* Save a5 */
    s32i   a6, a1, 4             /* Save a6 */
    s32i   a7, a1, 8             /* Save a7 */
    s32i   a8, a1, 12            /* Save a8 */

    /* Use a5 as base pointer to state_buf */
    .loc 1 25 0                  /* Source file: 'prempt_espx.S', line 25 */
    mov    a5, a2                /* a5 = state_buf */

    /* Save general-purpose registers into state_buf->registers */
    .loc 1 30 0                  /* Source file: 'prempt_espx.S', line 30 */
    s32i   a0, a5, 0             /* registers[0] */
    s32i   a1, a5, 4             /* registers[1] */
    s32i   a2, a5, 8             /* registers[2] */
    s32i   a3, a5, 12            /* registers[3] */
    /* Skip a4 (holds saved PS), save it later */
    s32i   a5, a5, 20            /* registers[5] (a5 is base pointer) */
    s32i   a6, a5, 24            /* registers[6] */
    s32i   a7, a5, 28            /* registers[7] */
    s32i   a8, a5, 32            /* registers[8] */
    s32i   a9, a5, 36            /* registers[9] */
    s32i   a10, a5, 40           /* registers[10] */
    s32i   a11, a5, 44           /* registers[11] */
    s32i   a12, a5, 48           /* registers[12] */
    s32i   a13, a5, 52           /* registers[13] */
    s32i   a14, a5, 56           /* registers[14] */
    s32i   a15, a5, 60           /* registers[15] */


    /* Save a4 (after we're done using it) */
    .loc 1 35 0                  /* Source file: 'prempt_espx.S', line 35 */
    s32i   a4, a5, 16            /* registers[4] (a4 holds saved PS) */

    /* Load options->use_epc_reg */
    .loc 1 40 0                  /* Source file: 'prempt_espx.S', line 40 */
    l32i   a6, a3, 4             /* a6 = options->use_epc_reg */

    /* Save the program counter */
    .loc 1 45 0                  /* Source file: 'prempt_espx.S', line 45 */
    beqz   a6, save_pc_normal    /* If use_epc_reg == 0, jump to save_pc_normal */

    /* Save EPC register into state_buf->pc */
    .loc 1 50 0                  /* Source file: 'prempt_espx.S', line 50 */
    rsr    a7, EPC1              /* Read EPC1 register into a7 */
    s32i   a7, a5, 64            /* state_buf->pc = EPC1 */

    /* Save stack pointer */
    .loc 1 55 0                  /* Source file: 'prempt_espx.S', line 55 */
    s32i   a1, a5, 68            /* state_buf->sp */

    /* Save stack_size from options */
    .loc 1 60 0                  /* Source file: 'prempt_espx.S', line 60 */
    j      save_stack            /* Jump to save_stack */

save_pc_normal:
    /* Save return address (from a0) into state_buf->pc */
    .loc 1 65 0                  /* Source file: 'prempt_espx.S', line 65 */
    s32i   a0, a5, 64            /* state_buf->pc = a0 */

    /* Save stack pointer */
    .loc 1 70 0                  /* Source file: 'prempt_espx.S', line 70 */
    s32i   a1, a5, 68            /* state_buf->sp */

save_stack:
    /* Save stack_size from options */
    .loc 1 75 0                  /* Source file: 'prempt_espx.S', line 75 */
    l32i   a6, a3, 0             /* a6 = options->stack_size */
    s32i   a6, a5, 76            /* state_buf->stack_size */

    /* Save the stack contents */
    .loc 1 80 0                  /* Source file: 'prempt_espx.S', line 80 */
    l32i   a7, a5, 72            /* a7 = state_buf->stack (destination pointer) */
    mov    a8, a1                /* a8 = current sp (source pointer) */

    /* Calculate the number of words to copy */
    .loc 1 85 0                  /* Source file: 'prempt_espx.S', line 85 */
    srli   a6, a6, 2             /* a6 = stack_size / 4 */

    beqz   a6, copy_stack_done   /* If stack_size == 0, skip copying */

copy_stack_loop:
    .loc 1 90 0                  /* Source file: 'prempt_espx.S', line 90 */
    l32i   a9, a8, 0             /* Load word from source */
    s32i   a9, a7, 0             /* Store word to destination */
    addi   a8, a8, 4             /* Increment source pointer */
    addi   a7, a7, 4             /* Increment destination pointer */
    addi   a6, a6, -1            /* Decrement counter */

    .loc 1 95 0                  /* Source file: 'prempt_espx.S', line 95 */
    bnez   a6, copy_stack_loop   /* Loop if counter not zero */

copy_stack_done:
    /* Restore saved registers */
    .loc 1 100 0                 /* Source file: 'prempt_espx.S', line 100 */
    l32i   a5, a1, 0             /* Restore a5 */
    l32i   a6, a1, 4             /* Restore a6 */
    l32i   a7, a1, 8             /* Restore a7 */
    l32i   a8, a1, 12            /* Restore a8 */
    addi   a1, a1, 16            /* Adjust stack pointer back */

    /* Re-enable interrupts by restoring PS */
    // wsr    a4, PS                /* Restore PS from a4 */
    // rsync                        /* Ensure PS is updated before proceeding */

    /* Return from the function */
    .loc 1 105 0                 /* Source file: 'prempt_espx.S', line 105 */
    ret

    /*------------------------------------------------------------------------
     * Function: __asm_espx_restore_task_context
     * Restores the task context from the state buffer provided.
     * Interrupts are disabled during this operation.
     *
     * Arguments:
     *   a2 - Pointer to dynsched_prempt_espx_state_buffer_t (state_buf)
     *------------------------------------------------------------------------*/
    .global __asm_espx_restore_task_context
__asm_espx_restore_task_context:
    /* Disable interrupts and save previous PS */
    // rsil   a4, 15                /* Disable interrupts, save PS to a4 */

    /* Save registers that will be modified */
    addi   a1, a1, -12           /* Adjust stack pointer to save 3 registers */
    s32i   a5, a1, 0             /* Save a5 */
    s32i   a6, a1, 4             /* Save a6 */
    s32i   a7, a1, 8             /* Save a7 */

    /* Use a5 as base pointer to state_buf */
    mov    a5, a2                /* a5 = state_buf */

    /* Restore general-purpose registers from state_buf->registers */
    l32i   a0, a5, 0             /* registers[0] */
    l32i   a1, a5, 4             /* registers[1] (stack pointer) */
    l32i   a2, a5, 8             /* registers[2] */
    l32i   a3, a5, 12            /* registers[3] */
    l32i   a4, a5, 16            /* registers[4] (a4 holds saved PS) */
    /* Skip a5 (it's our base pointer) */
    l32i   a6, a5, 24            /* registers[6] */
    l32i   a7, a5, 28            /* registers[7] */
    l32i   a8, a5, 32            /* registers[8] */
    l32i   a9, a5, 36            /* registers[9] */
    l32i   a10, a5, 40           /* registers[10] */
    l32i   a11, a5, 44           /* registers[11] */
    l32i   a12, a5, 48           /* registers[12] */
    l32i   a13, a5, 52           /* registers[13] */
    l32i   a14, a5, 56           /* registers[14] */
    l32i   a15, a5, 60           /* registers[15] */

    /* Restore a5 (after we're done using it) */
    l32i   a5, a5, 20            /* registers[5] */

    /* Restore stack pointer */
    l32i   a1, a2, 68            /* a1 = state_buf->sp */

    /* Restore stack contents */
    l32i   a6, a2, 76            /* a6 = state_buf->stack_size */
    l32i   a7, a2, 72            /* a7 = state_buf->stack (source pointer) */
    mov    a8, a1                /* a8 = current sp (destination pointer) */

    /* Calculate the number of words to copy */
    srli   a6, a6, 2             /* a6 = stack_size / 4 */

    beqz   a6, restore_stack_done /* If stack_size == 0, skip copying */

restore_stack_loop:
    l32i   a9, a7, 0             /* Load word from source */
    s32i   a9, a8, 0             /* Store word to destination */
    addi   a7, a7, 4             /* Increment source pointer */
    addi   a8, a8, 4             /* Increment destination pointer */
    addi   a6, a6, -1            /* Decrement counter */
    bnez   a6, restore_stack_loop /* Loop if counter not zero */

restore_stack_done:
    /* Restore program counter */
    l32i   a6, a2, 64            /* a6 = state_buf->pc */

    /* Restore saved registers */
    l32i   a5, a1, 0             /* Restore a5 */
    l32i   a6, a1, 4             /* Restore a6 */
    l32i   a7, a1, 8             /* Restore a7 */
    addi   a1, a1, 12            /* Adjust stack pointer back */

    /* Re-enable interrupts by restoring PS */
    // wsr    a4, PS                /* Restore PS from a4 */
    // rsync                        /* Ensure PS is updated before proceeding */

    /* Jump to the restored program counter */
    jx     a6                    /* Jump to address in a6 */

    /* This point is not reachable */
    nop


debug_msg_start:
    .asciz "Entering __asm_espx_save_task_context\n"