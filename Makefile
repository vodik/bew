CC = cc
CFLAGS = -Wall -pedantic -std=gnu99 -I./include -g
LDFLAGS = -lm

OBJ = reverb.o sine.o bew.o synth.o

all: bew

bew: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

%.o: %.c
	@echo CC $@
	@${CC} -o $@ -c ${CFLAGS} $<

clean:
	@echo cleaning up...
	@rm *.o bew

.PHONY: all clean
