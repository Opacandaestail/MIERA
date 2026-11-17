/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "dev/leds.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(parpadeo_1_process, "Parpadeo 1 Process");
PROCESS(parpadeo_2_process, "Parpadeo 2 Process");
PROCESS(timer_process, "Timer Process");
AUTOSTART_PROCESSES(&timer_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(timer_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Setup a periodic timer that expires after 10 seconds. */
  etimer_set(&timer, CLOCK_SECOND * 5);

  PROCESS_WAIT_UNTIL(etimer_expired(&timer));

  process_start(&parpadeo_1_process,data);
  process_start(&parpadeo_2_process,data);

  printf("Empieza\n");

  PROCESS_END();
}

PROCESS_THREAD(parpadeo_1_process, ev, data)
{
  static struct etimer t1;

	PROCESS_BEGIN();

	etimer_set(&t1, CLOCK_SECOND * 2);

	printf("Configurado LED1\n");

	while(1){
		PROCESS_WAIT_UNTIL(etimer_expired(&t1));
		etimer_reset(&t1);
		printf("Cambia LED1\n");
		leds_toggle(LEDS_RED);

	}

	PROCESS_END();
}

PROCESS_THREAD(parpadeo_2_process, ev, data)
{
  static struct etimer t2;

	PROCESS_BEGIN();

	etimer_set(&t2, CLOCK_SECOND * 3);

	printf("Configurado LED2\n");

	while(1){
		PROCESS_WAIT_UNTIL(etimer_expired(&t2));
		etimer_reset(&t2);
		printf("Cambia LED2\n");
		leds_toggle(LEDS_LED2);


	}

	PROCESS_END();
}

/*---------------------------------------------------------------------------*/
