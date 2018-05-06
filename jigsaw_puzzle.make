JIGSAW_PUZZLE_C_FLAGS=-O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

jigsaw_puzzle: jigsaw_puzzle.o
	gcc -o jigsaw_puzzle jigsaw_puzzle.o

jigsaw_puzzle.o: jigsaw_puzzle.c jigsaw_puzzle.make
	gcc -c ${JIGSAW_PUZZLE_C_FLAGS} -o jigsaw_puzzle.o jigsaw_puzzle.c

clean:
	rm -f jigsaw_puzzle jigsaw_puzzle.o
