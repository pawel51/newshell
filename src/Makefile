IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=bin
LDIR =../lib


_DEPS = shell.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = executing.o history.o main.o reading.o splitting.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: shell

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

clean:
	rm *.o