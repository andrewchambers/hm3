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