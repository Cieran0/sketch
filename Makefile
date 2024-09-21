build: sketch.c
	gcc -o sketch *.c -lraylib -lm

run: build
	./sketch