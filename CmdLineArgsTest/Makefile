CC = gcc
EXE = main.exe

%.exe : %.o
	$(CC) -o $@ $^

%.o : %.c
	$(CC) -c -o $@ $^

all: $(EXE)

clean: 
	rm -f *.exe *.o
