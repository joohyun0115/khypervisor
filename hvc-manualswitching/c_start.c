/*
 * Copyright (c) 2012 Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Linaro Limited nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 */

#include <stdint.h>
#include "monitor.h"
#include "uart_print.h"

#define read_cpsr()              ({ unsigned int rval; asm volatile(\
                                " mrs     %0, cpsr\n\t" \
                                : "=r" (rval) : : "memory", "cc"); rval;})
void c_start(void)
{
	uart_print("[secure] Starting...\n\r");
	uart_print_hex32( 0xF00DBEAF ); uart_print( "\n\r" );

	/* Initialize Monitor by installing Secure Monitor Call Execption handler */
	mon_init();

	mon_enter_hyp();	// Secure -> NS.Hyp -> hyp_main()

	/* ___ DEAD END ___ */
}

#ifndef BAREMETAL_GUEST
void nrm_loop(void) 
{
	int i = 0;
	unsigned int cpsr;
	
	uart_print("[nrm] enter\n\r");

	cpsr = read_cpsr();
	uart_print("[nrm] cpsr:"); uart_print_hex32(cpsr); uart_print("\n\r");

	for( i = 0; i < 10; i++ ) {
		uart_print("[nrm] ping 1\n\r");
		/* World Switch to Secure through Secure Monitor Call Exception */
		asm ("hvc #0xFFFE");		/* -> sec_loop() */
		uart_print("[nrm] yield 2\n\r");
		asm ("hvc #0xFFFD");		/* -> sec_loop() */
		uart_print("[nrm] back 3\n\r");
	}
	uart_print("[nrm] done\n\r");
}
#endif
