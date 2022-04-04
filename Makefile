# Luan Machado Bernardt | GRR20190363

CC     = gcc
CFLAGS =

PROG = flood
OBJS = flood.o queue.o heuristic.o

.PHONY: clean purge all

all: $(PROG)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

$(PROG) : % :  $(OBJS) %.o
	$(CC) -o $@ $^

clean:
	@rm -f *.o

purge:   
	@rm -f $(PROG)