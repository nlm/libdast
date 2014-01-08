LIBSO=libdast.so
LIBA=libdast.a
SRC=uhash.c uring.c mtqueue.c
CFLAGS=-Wall -fPIC -O2
LDFLAGS=-shared
OBJ=$(SRC:.c=.o)

all: $(LIBSO) $(LIBA)

$(LIBSO): $(OBJ)
	$(CC) $(OBJ) -o $(LIBSO) $(LDFLAGS)

$(LIBA): $(OBJ)
	ar -r $(LIBA) $(OBJ)

clean:
	rm -f $(OBJ)

distclean: clean
	rm -f $(LIBA) $(LIBSO)

.PHONY: all clean distclean
