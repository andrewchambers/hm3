#ifndef HM3_H
#define HM3_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <tommath.h> /* XXX We want a library we can embed. */

enum hm3_type_kind {
    HM3_TYPE_NUMBER,
    HM3_TYPE_TYPE,
};

struct hm3_type {
    enum hm3_type_kind kind;
};

struct hm3_gcobject {
    uintptr_t rc;
    struct hm3_gcobject *next;
    struct hm3_gcobject *prev;
    struct hm3_type *type;
};

#define HM3_SMALL_NUMBER_MIN -4611686018427387904
#define HM3_SMALL_NUMBER_MAX 4611686018427387905

struct hm3_small_number {
    int64_t value : 63; /* TODO 32 bit platforms. */
    int is_small : 1;
};

struct hm3_big_number {
    struct hm3_gcobject gcobject;
    mp_int value;
};

union hm3_number {
    struct hm3_small_number small;
    struct hm3_big_number *big;
};

typedef void (*hm3_oom_cb)();

struct hm3_vm {
    hm3_oom_cb out_of_memory; /* Don't invoke directly, use hm3_out_of_memory
                                 which is noreturn. */
    struct hm3_gcobject *head_gcobject, *tail_gcobject;
};

typedef union {
    struct hm3_gcobject *gcobject;
    union hm3_number number;
} hm3_value;

hm3_value hm3_incref(hm3_value v);
void hm3_decref(hm3_value v);

/*
 * Initialize the hm3 virtual machine in thread local storage.
 *
 * returns -1 on failure.
 */
int hm3_vm_init(struct hm3_vm *vm);

/* Invokes the vm's out of memory handler and aborts execution. */
noreturn void hm3_out_of_memory(struct hm3_vm *vm);

/*
 * Allocated a garbaged collected object on the heap, each object has an
 * hm3_gcobject header. Will never fail, instead will invoke hm3_out_of_memory
 * if there is insufficient memory.
 */
void *hm3_alloc_gcobject(struct hm3_vm *vm, size_t sz);

/* Free resources associated with the hm3 vm. */
void hm3_vm_finish(struct hm3_vm *vm);

#endif
