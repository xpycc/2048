CC = clang
override CFLAGS += -std=c99

2048: main.o default_strategy.o strategy.o
	$(CC)  -o $@ $^ -lncurses

default_strategy.o: default_strategy.c strategy.h
	$(CC) $(CFLAGS) -c default_strategy.c

main.o: main.c strategy.h
	$(CC) $(CFLAGS) -c main.c

strategy.o: strategy.c strategy.h
	$(CC) $(CFLAGS) -c strategy.c

.PHONY: objclean clean
objclean:
	rm -f *.o

clean:
	rm -f *.o 2048
