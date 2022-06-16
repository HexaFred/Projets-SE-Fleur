#include <stdio.h>		/* 	printf() 					*/
#include <sys/types.h>	/** Primitive System Data Types	*/
#include <sys/ipc.h>	/*	Interprocess communication	*/
#include <sys/shm.h>	/*  Shared memory			   **/

#include "shm-common.h"
#include "shm-flower.h"

int create_key(char* pathname, int proj_id, key_t *key){
	*key = ftok(pathname, proj_id);
	if (*key == -1){
		return 1;
    }
	return 0;
}

void print_settings(char* pathname, int proj_id){
	printf("proj_id = \"%d\"\n", proj_id);
	printf("pathname = \"%s\"\n", pathname);
}