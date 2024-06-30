/*
Name: Yash Kapoor
*/

#ifndef SEMUN_H
#define SEMUN_H 

#include <sys/types.h>
#include <sys/sem.h>

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

#endif
