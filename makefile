all:
	g++ main.cpp clock.h -lncurses -pthread -Wall -o timer

clean:
	rm -rf *.gch a.out
