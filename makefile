All: project

project: main.o byte.o general.o list.o segment.o
	gcc main.o byte.o general.o list.o segment.o -o Cproject

main.o: main.c Byte.h General.h List.h Segment.h Struct.h
	gcc -c main.c

byte.o: byte.c Byte.h General.h List.h Segment.h Struct.h
	gcc -c byte.c

general.o: general.c Byte.h General.h List.h Segment.h Struct.h
	gcc -c general.c

list.o: list.c Byte.h General.h List.h Segment.h Struct.h
	gcc -c list.c

segment.o: segment.c Byte.h General.h List.h Segment.h Struct.h
	gcc -c segment.c

clean:
	rm project main.o byte.o general.o list.o segment.o
