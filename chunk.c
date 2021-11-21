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
        chunk->constants = reallocarray(chunk->constants,
            chunk->constants_capacity ? 8 : chunk->constants_capacity * 2,
            sizeof(hm3_value));
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
        chunk->opcodes = reallocarray(chunk->opcodes,
            (chunk->opcodes_capacity ? 32 : chunk->opcodes_capacity * 2),
            sizeof(op));
        if (!chunk->opcodes)
            hm3_out_of_memory(vm);
    }
    chunk->opcodes[chunk->opcodes_count] = op;
    return chunk->opcodes_count++;
}