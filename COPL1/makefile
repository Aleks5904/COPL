CC = g++

CompileParms = -c -Wall -std=c++11 -O2

OBJS = main.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o CPL1

clean:
	rm -rf *.o CPL1

main.o: main.cc
	$(CC) $(CompileParms)  main.cc
