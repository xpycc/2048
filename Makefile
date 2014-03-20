CC = clang
CXX = clang++
override CFLAGS += -std=c99
# override CXXFLAGS += -std=c++11

2048: main.o default_strategy.o strategy.o
	$(CXX) -o $@ $^ -lncurses

default_strategy.o: default_strategy.c strategy.h
	$(CC) $(CFLAGS) -c default_strategy.c

main.o: main.c strategy.h
	$(CC) $(CFLAGS) -c main.c

strategy.o: strategy.cc strategy.h
	$(CXX) $(CXXFLAGS) -c strategy.cc

.PHONY: objclean clean
objclean:
	rm -f *.o

clean:
	rm -f *.o 2048
