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
#include "arch/platform/nrf52840/common/temperature-sensor.h"
#include "nrf_temp.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/

static int
value(void)
{
  int32_t volatile temp;

  NRF_TEMP->TASKS_START = 1;
  /* nRF52832 datasheet: one temperature measurement takes typically 36 us */
  RTIMER_BUSYWAIT_UNTIL(NRF_TEMP->EVENTS_DATARDY, RTIMER_SECOND * 72 / 1000000);
  NRF_TEMP->EVENTS_DATARDY = 0;
  temp = nrf_temp_read();
  NRF_TEMP->TASKS_STOP = 1;

  return temp;
}

PROCESS(read_temp, "Read Temperature Process");
PROCESS(timer_process, "Timer Process");
AUTOSTART_PROCESSES(&timer_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(timer_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 3);

  process_start(&read_temp,data);

  //nrf_temp_init();

  while(1){

	PROCESS_WAIT_UNTIL(etimer_expired(&timer));
	etimer_reset(&timer);
	process_start(&read_temp,data);
  }

  PROCESS_END();
}

PROCESS_THREAD(read_temp,ev,data)
{   
	PROCESS_BEGIN();

	int t = value();

	int tint = t >> 2;

	int dec = t & (0x3);

	printf("%d,%d ºC\n",tint,dec);

	process_exit(&read_temp);
	
	PROCESS_END();
  
}

/*---------------------------------------------------------------------------*/