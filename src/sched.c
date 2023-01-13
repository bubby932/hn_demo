#ifndef HACKNET_SCHED
#define HACKNET_SCHED

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/string.c"
#include "lib/kutils.c"
#include "lib/mm.c"

volatile bool sched_active = false;

void init_sched() {

}

#endif