#ifndef HACKNET_SCHED
#define HACKNET_SCHED

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/string.cpp"
#include "lib/kutils.cpp"
#include "lib/irq.cpp"
#include "lib/mm.cpp"

void sched_init() {

}

extern "C"
void sched_tick_irq(void) {
    // TODO
    eoi(0);
}

#endif