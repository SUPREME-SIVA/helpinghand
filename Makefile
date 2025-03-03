CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: addition test_addition

addition: addition.c
	$(CC) $(CFLAGS) -o addition addition.c

test_addition: test_addition.c
	$(CC) $(CFLAGS) -o test_addition test_addition.c

test: test_addition
	./test_addition

clean:
	rm -f addition test_addition
