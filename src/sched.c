#ifndef HACKNET_SCHED
#define HACKNET_SCHED

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/string.c"
#include "lib/kutils.c"
#include "lib/mm.c"

typedef struct _State {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;

    uint32_t esi;
    uint32_t edi;

    uint32_t esp;
    uint32_t ebp;

    uint32_t eip;

    uint32_t eflags;

    uint32_t cr0;
    
} __attribute__((packed)) TaskState;

typedef struct _Task {
    struct _Task *next;


} Task;

typedef struct _Process {
    struct _Process *previous;
    struct _Process *next;

    uint32_t pid;

    Task *tasks;
} Process;

static volatile Process *root;
static volatile Process *current;

volatile bool sched_active = false;

void init_sched() {

}

static void free_proc(Process *p) {
    if(current == p)
        current = p->next;

    Task *current = p->tasks;
    while(current != NULL) {
        Task *next = current->next;

        kfree(current);

        current = next;
    }

    kfree(p);
}

void kill(Process *p) {
    if(p == root) {
        Process *next = p->next;
        next->previous = NULL;
        root = next;
    } else {
        Process *prev = p->previous;
        prev->next = p->next;

        Process *next = p->next;
        next->previous = prev;
    }
    free_proc(p);
}


#endif