.POSIX:
.PHONY: all clean

-include config.mk

AMALG=\
  vm.c

OBJ=\
	vm.o\
	number.o\
	main.o\

all: hm3

hm3: $(OBJ)
	$(CC) $(LDFLAGS) -ltommath -o $@ $(OBJ)

main.o vm.o: hm3.h

hm3_amalgamation.c: $(AMALG)
	cat $(AMALG) > $@

fmt:
	clang-format \
	  -style="{BasedOnStyle: WebKit,\
	           AlwaysBreakAfterReturnType: TopLevelDefinitions,\
	           ColumnLimit: 80,\
	           PointerAlignment: Right}"\
	  -i *.c *.h

clean:
	rm -f $(OBJ) hm3
