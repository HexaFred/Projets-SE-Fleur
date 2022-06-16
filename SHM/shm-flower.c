#include <stdlib.h>	/* strlen()				*/
#include <string.h>	/* strcmp(), strcpy()	*/
#include <stdio.h>	/* printf()				*/
#include <time.h> 	/* time 				*/

#include "shm-flower.h"

int shm_flower_copy(shm_flower_t source, shm_flower_t *target) {
	if (shm_flower_is_empty(*target)==0) 
		return shm_flower_error_not_empty;
	
	if (shm_flower_set_color(target, source.color) == shm_flower_error_color)
		return shm_flower_error_color;

	if (shm_flower_set_name(target, source.name) == shm_flower_error_name)
		return shm_flower_error_name;
	return 0;
}

void shm_flower_empty(shm_flower_t *flower) {
	flower->color[0] = '\0';
	flower->name[0] = '\0';
} 

int shm_flower_is_empty(shm_flower_t flower) {
	if ((strlen(flower.color)==0)&&(strlen(flower.name)==0))
		return 1;
	return 0;
}

void shm_flower_print(shm_flower_t flower) {
	time_t timestamp = time(NULL);
	struct tm * info = localtime (&timestamp);
	/* YYYY-MM-DD HH:MM:SS: BRAND: MODEL */
	printf("%04d-%02d-%02d %02d:%02d:%02d: %s: %s\n", info->tm_year+1900, info->tm_mon+1, info->tm_mday, info->tm_hour, info->tm_min, info->tm_sec, flower.color, flower.name);
}

int shm_flower_set_color(shm_flower_t *flower, const char *color) {
	if (strlen(color) > SHM_FLOWER_COLOR_SIZE - 1)
		return shm_flower_error_color;
	else 
		strcpy(flower->color, color);
	return 0;
}

int shm_flower_set_name(shm_flower_t *flower, const char *name) {
	if (strlen(name) > SHM_FLOWER_NAME_SIZE - 1)
		return shm_flower_error_name;
	else 
		strcpy(flower->name, name);
	return 0;
}