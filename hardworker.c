#include "contiki.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "sys/etimer.h"
#include <stdio.h>
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_DBG

/*---------------------------------------------------------------------------*/
PROCESS(led_process, "LED Toggle P.ID:03");
PROCESS(udp_process, "Simulated UDP P.ID:34");
PROCESS(sensor_process, "Sensor Event P.ID:06");
PROCESS(dummy_printer_process, "Dummy Printer P.ID:61");
AUTOSTART_PROCESSES(&led_process, &udp_process, &sensor_process, &dummy_printer_process);
/*---------------------------------------------------------------------------*/

/* Simulated UDP - Just posts itself for demo */
static struct etimer udp_timer;
PROCESS_THREAD(udp_process, ev, data)
{
  PROCESS_BEGIN();
  etimer_set(&udp_timer, CLOCK_SECOND / 4);

  while(1) {
    PROCESS_WAIT_EVENT();
    if (ev == PROCESS_EVENT_TIMER && etimer_expired(&udp_timer)) {
      LOG_DBG("[UDP] Sending simulated UDP packet\n"); //OS-Process Scheduling
      process_post(&sensor_process, PROCESS_EVENT_POLL, NULL);
      etimer_reset(&udp_timer);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

static struct etimer led_timer;
PROCESS_THREAD(led_process, ev, data)
{
  PROCESS_BEGIN();
  etimer_set(&led_timer, CLOCK_SECOND / 4);

  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER && etimer_expired(&led_timer)) {
      leds_toggle(LEDS_ALL);
      LOG_DBG("[LED] LED Toggled\n"); //OS-Process Scheduling
      etimer_reset(&led_timer);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(sensor_process, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);

  while(1) {
    PROCESS_WAIT_EVENT();
    if (ev == sensors_event && data == &button_sensor) {
      LOG_DBG("[SENSOR] Button pressed\n"); //OS-Process Scheduling
    } else if (ev == PROCESS_EVENT_POLL) {
      LOG_DBG("[SENSOR] Received UDP-triggered poll\n"); //OS-Process Scheduling
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

static struct etimer loop_timer;
PROCESS_THREAD(dummy_printer_process, ev, data)
{
  PROCESS_BEGIN();
  etimer_set(&loop_timer, CLOCK_SECOND / 4); // runs often

  while(1) {
    PROCESS_WAIT_EVENT();
    if (ev == PROCESS_EVENT_TIMER && etimer_expired(&loop_timer)) {
      LOG_DBG("[PRINTER] Dummy printing..\n"); //OS-Process Scheduling
      etimer_reset(&loop_timer);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/