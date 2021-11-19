#include "hm3.h"

int
main(int argc, char **argv)
{
    struct hm3_vm vm;

    if (hm3_init_vm(&vm) == -1) {
        fputs("unable to initialize virtual machine, exiting\n", stderr);
        exit(1);
    }

    hm3_deinit_vm(&vm);
    return 0;
}