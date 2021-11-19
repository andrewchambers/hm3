#include "hm3.h"

int
hm3_init_vm(struct hm3_vm *vm)
{
    struct hm3_gcobject;

    memset(vm, 0, sizeof(struct hm3_vm));
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

static struct hm3_gcobject *
alloc_gcobject(struct hm3_vm *vm, size_t size)
{
    struct hm3_gcobject *obj;

    if (size > (SIZE_MAX - sizeof(struct hm3_gcobject)))
        return NULL;

    obj = malloc(sizeof(struct hm3_gcobject) + size);
    if (!obj)
        return NULL;

    obj->rc = 1;
    obj->prev = vm->tail_gcobject;
    obj->next = NULL;
    vm->tail_gcobject->next = obj;
    vm->tail_gcobject = obj;

    return obj;
}

void
hm3_deinit_vm(struct hm3_vm *vm)
{
    struct hm3_gcobject *obj;

    for (obj = vm->head_gcobject; obj; obj = obj->next)
        free_gcobject(obj);
}
