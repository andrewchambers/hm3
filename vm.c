/*
 * This file implements core operations on the hm3 virtual machine.
 */

#include "hm3.h"

static void
default_oom_handler(void)
{
    fputs("out of memory\n", stderr);
    exit(111);
}

noreturn void
hm3_out_of_memory(struct hm3_vm *vm)
{
    vm->out_of_memory();
    abort();
}

int
hm3_vm_init(struct hm3_vm *vm)
{
    struct hm3_gcobject;

    memset(vm, 0, sizeof(struct hm3_vm));
    vm->out_of_memory = default_oom_handler;
    return 0;
}

static void
free_gcobject(struct hm3_gcobject *obj)
{
    if (obj->next)
        obj->next->prev = obj->prev;
    if (obj->prev)
        obj->prev->next = obj->next;

    free(obj);
}

hm3_value
hm3_incref(hm3_value v)
{
    /* Low bit set, not a heap object .  */
    if (((uintptr_t)v.gcobject) & 1)
        return v;

    v.gcobject->rc += 1;
    return v;
}

void
hm3_decref(hm3_value v)
{
    v.gcobject->rc -= 1;
    if (v.gcobject->rc == 0)
        free_gcobject(v.gcobject);
}

void *
hm3_alloc_gcobject(struct hm3_vm *vm, size_t size)
{
    struct hm3_gcobject *obj;

    obj = malloc(sizeof(struct hm3_gcobject));
    if (!obj)
        hm3_out_of_memory(vm);

    obj->rc = 1;
    obj->prev = vm->tail_gcobject;
    obj->next = NULL;
    vm->tail_gcobject->next = obj;
    vm->tail_gcobject = obj;

    return (void *)obj;
}

void
hm3_vm_finish(struct hm3_vm *vm)
{
    struct hm3_gcobject *obj;

    for (obj = vm->head_gcobject; obj; obj = obj->next)
        free_gcobject(obj);
}
