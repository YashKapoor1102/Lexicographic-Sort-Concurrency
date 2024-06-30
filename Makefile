CSORT: CSORT.o 
	gcc CSORT.o -o CSORT

CSORT.o: CSORT.c CSORT.h semun.h
	gcc -c CSORT.c

clean:
	rm *.o CSORT


