/*
 * This file implements operations on bytecode chunks,
 * a chunk is an array of bytecode and associated data.
 */
#include "hm3.h"

struct hm3_chunk *
hm3_chunk_create(struct hm3_vm *vm)
{
    struct hm3_chunk *chunk;

    chunk = malloc(sizeof(struct hm3_chunk));
    if (!chunk)
        hm3_out_of_memory(vm);

    memset(chunk, 0, sizeof(struct hm3_chunk));

    return chunk;
}

void
hm3_chunk_destroy(struct hm3_vm *vm, struct hm3_chunk *chunk)
{
    size_t i;

    for (i = 0; i < chunk->constants_count; i++)
        hm3_decref(chunk->constants[i]);

    free(chunk->opcodes);
    free(chunk->constants);
    free(chunk);
}

size_t
hm3_chunk_add_constant(struct hm3_vm *vm, struct hm3_chunk *chunk, hm3_value v)
{
    if (chunk->constants_capacity == chunk->constants_count) {
        chunk->constants_capacity
            = chunk->constants_capacity ? 8 : chunk->constants_capacity * 2;
        chunk->constants = reallocarray(
            chunk->constants, chunk->constants_capacity, sizeof(hm3_value));
        if (!chunk->constants)
            hm3_out_of_memory(vm);
    }
    chunk->constants[chunk->constants_count] = hm3_incref(v);
    return chunk->constants_count++;
}

size_t
hm3_chunk_add_op(struct hm3_vm *vm, struct hm3_chunk *chunk, uint8_t op)
{
    if (chunk->opcodes_capacity == chunk->opcodes_count) {
        chunk->opcodes_capacity
            = chunk->opcodes_capacity ? 32 : chunk->opcodes_capacity * 2;
        chunk->opcodes
            = reallocarray(chunk->opcodes, chunk->opcodes_capacity, sizeof(op));
        if (!chunk->opcodes)
            hm3_out_of_memory(vm);
    }
    chunk->opcodes[chunk->opcodes_count] = op;
    return chunk->opcodes_count++;
}

void
hm3_chunk_add_push_value(
    struct hm3_vm *vm, struct hm3_chunk *chunk, hm3_value v)
{
    size_t constant;

    constant = hm3_chunk_add_constant(vm, chunk, v);
    if (constant <= 255) {
        hm3_chunk_add_op(vm, chunk, OP_CONSTANT8);
        hm3_chunk_add_op(vm, chunk, (uint8_t)constant);
    } else if (constant <= 65535) {
        hm3_chunk_add_op(vm, chunk, OP_CONSTANT16);
        hm3_chunk_add_op(vm, chunk, ((uint16_t)constant) & 0xff);
        hm3_chunk_add_op(vm, chunk, ((uint16_t)constant & 0xff00) >> 8);
    } else {
        hm3_out_of_memory(vm); // XXX Better error or handle it.
    }
}

void
hm3_chunk_trim(struct hm3_vm *vm, struct hm3_chunk *chunk)
{
    chunk->opcodes = reallocarray(chunk->opcodes, chunk->opcodes_count, 1);
    if (!chunk->opcodes)
        hm3_out_of_memory(vm);
    chunk->opcodes_capacity = chunk->opcodes_count;

    chunk->constants = reallocarray(
        chunk->constants, chunk->constants_count, sizeof(hm3_value));
    if (!chunk->constants)
        hm3_out_of_memory(vm);
    chunk->constants_capacity = chunk->constants_count;
}