.POSIX:
.PHONY: all clean

-include config.mk

OBJ=\
	main.o\
	vm.o\

all: hm3

hm3: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

main.o vm.o: hm3.h

fmt:
	clang-format \
	  -style="{BasedOnStyle: WebKit,\
	           AlwaysBreakAfterReturnType: TopLevelDefinitions,\
	           ColumnLimit: 80,\
	           PointerAlignment: Right}"\
	  -i *.c *.h

clean:
	rm -f $(OBJ) hm3
