CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter
CC = gcc

PROGRAMS = exec

.PHONY:	all clean

all: $(PROGRAMS)

exec: main.c list_str.o instantane.o instantane.h list_str.h
	$(CC) -o $@ $(CFLAGS) $^

list_str.o: list_str.c list_str.h
	$(CC) -c list_str.c $(CFLAGS)

instantane.o: instantane.c instantane.h list_str.h
	$(CC) -c instantane.c $(CFLAGS)

#Ou plus simplement
#%.o:%.c %.h
#	$(CC)  $(GCC_FLAGS) -c  $<

clean:
	rm -f *.o *~ $(PROGRAMS)