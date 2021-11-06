/*
 * EABI wrappers from the udivmod and idivmod functions
 */

	.globl ret_uidivmod_values
	.globl ret_idivmod_values

/*
 * signed ret_idivmod_values(signed quot, signed rem);
 * return quotient and remaining the EABI way (regs r0,r1)
 */
ret_idivmod_values:
        bx lr

/*
 * unsigned ret_uidivmod_values(unsigned quot, unsigned rem);
 * return quotient and remaining the EABI way (regs r0,r1)
 */
ret_uidivmod_values:
        bx      lr
