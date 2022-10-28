# CS5375 Computer Systems Organization and Architecture
# Course Project
# Cache Simulator Design and Development
# FALL 2022
# Orginal Simulator By Yong Chen Modified by Stephen Devaney


 CC = gcc
EXE = multi_level_cachesim.exe

%.exe : %.o
	$(CC) -o $@ $^ -lm

%.o : %.c
	$(CC) -c -o $@ $^

all: $(EXE)

clean: 
	rm -f *.exe *.o