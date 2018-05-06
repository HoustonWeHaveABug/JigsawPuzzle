JIGSAW_PUZZLE_GEN_C_FLAGS=-O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

jigsaw_puzzle_gen: jigsaw_puzzle_gen.o
	gcc -o jigsaw_puzzle_gen jigsaw_puzzle_gen.o

jigsaw_puzzle_gen.o: jigsaw_puzzle_gen.c jigsaw_puzzle_gen.make
	gcc -c ${JIGSAW_PUZZLE_GEN_C_FLAGS} -o jigsaw_puzzle_gen.o jigsaw_puzzle_gen.c

clean:
	rm -f jigsaw_puzzle_gen jigsaw_puzzle_gen.o
