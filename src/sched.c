#ifndef HACKNET_SCHED
#define HACKNET_SCHED

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/string.c"
#include "lib/kutils.c"
#include "lib/irq.c"
#include "lib/mm.c"

void sched_init() {

}

void sched_tick_irq(void) {
    serial_writestring("sched tick 00\n\r");
    // kpanic("[sched] Scheduler tick fired w/o handler!\n");
    eoi(0);
}

#endif