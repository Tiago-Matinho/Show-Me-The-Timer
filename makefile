all:
	g++ main.cpp clock.h -lncurses -Wall -o timer

clean:
	rm -rf *.gch a.out
