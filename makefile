all:
	g++ -Wall -lncurses main.cpp clock.h -o timer

clean:
	rm -rf *.gch a.out
