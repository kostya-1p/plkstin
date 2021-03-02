all: compile

compile: chello

chello: curse.o
	gcc curse.o -lncurses -o chello

curse.o: curse.c
	gcc -c -g curse.c

clean:
	rm *.o chello

install: chello
	cp chello /usr/local/bin/chello

deinstall uninstall:
	rm /usr/local/bin/chello
