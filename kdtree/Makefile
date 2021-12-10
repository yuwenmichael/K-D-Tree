map1:main.o kdtree.o process_data.o read_input_closest.o
	gcc -Wall -o map1 main.o kdtree.o process_data.o read_input_closest.o -lm

map2: main.o kdtree.o process_data.o read_input_radius.o
	gcc -Wall -o map2 main.o kdtree.o process_data.o read_input_radius.o -lm

main.o:main.c
	gcc -Wall -c -o main.o main.c -lm

kdtree.o: kdtree.c kdtree.h
	gcc -Wall -c -o kdtree.o kdtree.c -lm

process_data.o: process_data.c process_data.h
	gcc -Wall -c -o process_data.o process_data.c -lm

read_input_closest.o: read_input_closest.c read_input_closest.h
	gcc -Wall -c -o read_input_closest.o read_input_closest.c -lm

read_input_radius.o: read_input_radius.c read_input_radius.h
	gcc -Wall -c -o read_input_radius.o read_input_radius.c -lm