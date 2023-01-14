#ifndef HACKNET_SCHED
#define HACKNET_SCHED

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/string.c"
#include "lib/kutils.c"
#include "lib/mm.c"

void init_sched() {

}

void sched_tick_irq(void) {
    kpanic("[sched] Scheduler tick fired w/o handler!\n");
    eoi(0);
}

#endif