CC=gcc

all: main 

main: main.o parser.o scanner.o treePrint.o buildTree2.o semantics.o
	gcc -o comp main.o parser.o scanner.o treePrint.o buildTree2.o semantics.o
main.o: main.c
	gcc -c main.c
parser.o: parser.c 
	gcc -c parser.c 
scanner.o: scanner.c
	gcc -c scanner.c
treePrint.o: treePrint.c
	gcc -c treePrint.c
buildTree2.o: buildTree2.c
	gcc -c buildTree2.c
semantics.o: semantics.c
	gcc -c semantics.c
clean:
	rm *.o comp temp.fs16
