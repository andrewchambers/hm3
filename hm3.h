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

typedef union {
    struct hm3_gcobject *gcobject;
} hm3_value;

hm3_value hm3_incref(hm3_value v);
void hm3_decref(hm3_value v);

struct hm3_gcobject {
    uint64_t rc;
    struct hm3_gcobject *next;
    struct hm3_gcobject *prev;
    struct hm3_type *type;
    uint8_t data[0];
};

struct hm3_vm {
    struct hm3_gcobject *head_gcobject, *tail_gcobject;
};

/*
  Initialize the hm3 virtual machine in thread local storage.

  returns -1 on failure.
*/
int hm3_init_vm(struct hm3_vm *vm);

/* Free resources associated with the hm3 vm. */
void hm3_deinit_vm(struct hm3_vm *vm);
