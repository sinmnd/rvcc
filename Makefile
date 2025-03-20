CC=clang
CFLAGS=-std=c11 -g -fno-common

rvcc: main.o
	$(CC) $(CFLAGS) -o rvcc main.o

test: rvcc
	./test.sh

clean:
	rm -f rvcc *.o *.s tmp* a.out

.PHONY: clean test
