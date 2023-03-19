CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter
CC = gcc

PROGRAMS = exec

.PHONY:	all clean

all: $(PROGRAMS)

exec: main.c list_utility.o instantane.o file_utility.o hash_utility.o instantanewt.o workfile.o worktree.o instantane.h list_utility.h file_utility.h hash_utility.h instantanewt.h workfile.h worktree.h
	$(CC) -o $@ $(CFLAGS) $^

list_utility.o: list_utility.c list_utility.h
	$(CC) -c list_utility.c $(CFLAGS)

file_utility.o: file_utility.c file_utility.h list_utility.h
	$(CC) -c file_utility.c $(CFLAGS)

hash_utility.o: hash_utility.c hash_utility.h
	$(CC) -c hash_utility.c $(CFLAGS)

instantane.o: instantane.c instantane.h list_utility.h
	$(CC) -c instantane.c $(CFLAGS)

instantanewt.o: instantanewt.c instantanewt.h instantane.h worktree.h
	$(CC) -c instantanewt.c $(CFLAGS)

workfile.o: workfile.c workfile.h
	$(CC) -c workfile.c $(CFLAGS)

worktree.o: worktree.c worktree.h workfile.h
	$(CC) -c worktree.c $(CFLAGS)

#Ou plus simplement
#%.o:%.c %.h
#	$(CC)  $(GCC_FLAGS) -c  $<

clean:
	rm -f *.o *~ $(PROGRAMS)