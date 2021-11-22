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

void
hm3_vm_stack_push(struct hm3_vm *vm, hm3_value v)
{
    if (vm->stack_capacity == vm->stack_count) {
        vm->stack_capacity = vm->stack_capacity ? 512 : vm->stack_capacity * 2;
        vm->stack
            = reallocarray(vm->stack, vm->stack_capacity, sizeof(hm3_value));
        if (!vm->stack)
            hm3_out_of_memory(vm);
        vm->stack[vm->stack_count++] = v;
    }
}

hm3_value
hm3_vm_stack_pop(struct hm3_vm *vm)
{
    return vm->stack[--(vm->stack_count)];
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
hm3_vm_step(struct hm3_vm *vm)
{
    switch (*(vm->ip++)) {
    case OP_CONSTANT8: {
        size_t cidx;

        cidx = (size_t)(*(vm->ip++));
        hm3_vm_stack_push(vm, hm3_incref(vm->chunk->constants[cidx]));
        break;
    }
    case OP_CONSTANT16: {
        size_t cidx;

        cidx = (size_t)(*(vm->ip++)) | (size_t)(*(vm->ip++) << 8);
        hm3_vm_stack_push(vm, hm3_incref(vm->chunk->constants[cidx]));
        break;
    }
    default:
        abort();
    }
}

void
hm3_vm_finish(struct hm3_vm *vm)
{
    struct hm3_gcobject *obj;

    if (vm->chunk)
        hm3_chunk_destroy(vm, vm->chunk);

    while (vm->stack_count)
        hm3_decref(vm->stack[--vm->stack_count]);

    for (obj = vm->head_gcobject; obj; obj = obj->next)
        free_gcobject(obj);

    free(vm->stack);
}
