SRC=$(wildcard *.c)
DST=$(patsubst %.c,%,$(SRC))

default: $(DST)

register: register.c handler.S
	gcc -o $@ $^

unregister: unregister.c
	gcc -o $@ $^

clean:
	rm -rf $(DST)
