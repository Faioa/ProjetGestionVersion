CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter
CC = gcc

PROGRAMS = exec

.PHONY:	all clean

all: $(PROGRAMS)

exec: main.c list_str.o list_dir.o list_dir.h list_str.h
	$(CC) -o $@ $(CFLAGS) $^

list_str.o: list_str.c list_str.h
	$(CC) -c list_str.c $(CFLAGS)

list_dir.o: list_dir.c list_dir.h list_str.h
	$(CC) -c list_dir.c $(CFLAGS)

#Ou plus simplement
#%.o:%.c %.h
#	$(CC)  $(GCC_FLAGS) -c  $<

clean:
	rm -rf *.o *~ $(PROGRAMS) snapshot