CC=g++
CFLAGS=-Wall -Wextra -Werror
LDFLAGS=-pthread

all: echo b

echo: echo.cpp
	$(CC) $(CFLAGS) -o echo echo.cpp

b: b.cpp
	$(CC) $(CFLAGS) -o b b.cpp

clean:
	rm -f echo b *.o

run_echo:
	./echo

run_b:
	./b
