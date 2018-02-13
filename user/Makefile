SRC=$(wildcard *.c)
DST=$(patsubst %.c,%,$(SRC))

default: $(DST)

$(DST): $(SRC) handler.S
	gcc -o $@ $^

clean:
	rm -rf $(DST)
