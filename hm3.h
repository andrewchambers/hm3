#ifndef HM3_H
#define HM3_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
};

union hm3_number {
    struct hm3_small_number small;
    struct hm3_big_number *big;
};

struct hm3_vm {
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

/* Free resources associated with the hm3 vm. */
void hm3_vm_finish(struct hm3_vm *vm);

#endif
