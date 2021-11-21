#include "hm3.h"

int
main(int argc, char **argv)
{
    struct hm3_vm vm;

    if (hm3_vm_init(&vm) == -1) {
        fputs("unable to initialize virtual machine, exiting\n", stderr);
        exit(1);
    }

    struct hm3_chunk *chunk = hm3_chunk_create(&vm);
    hm3_chunk_destroy(&vm, chunk);

    hm3_vm_finish(&vm);
    return 0;
}
