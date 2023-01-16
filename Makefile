CC=gcc
CFLAGS=-O3 -lcurl -ljson-c
OUT=val.o net.o api.o

all: val

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

val: $(OUT)
	$(CC) -g -o val $(OUT) $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o val