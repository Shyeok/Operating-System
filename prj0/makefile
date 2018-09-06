testlib : main.o list.o hash.o bitmap.o
	gcc -o testlib main.o list.o hash.o bitmap.o
main.o : main.c list.h hash.h bitmap.h
	gcc -c main.c 
hash.o : hash.c hash.h list.h
	gcc -c hash.c
list.o : list.c list.h
	gcc -c list.c
bitmap.o : bitmap.c bitmap.h limits.h round.h
	gcc -c bitmap.c
clean :
	rm main.o list.o hash.o bitmap.o testlib
