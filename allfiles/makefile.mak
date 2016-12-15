//This file is used as a template of how to write simple makefiles.
CC=gcc

proj3: proj3.o
	gcc  -o proj3 proj3.o
proj3.o: proj3.c
	gcc -c proj3.o proj3.c
