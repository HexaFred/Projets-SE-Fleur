#include <stdlib.h> /* strlen()				*/
#include <string.h> /* strcmp(), strcpy()	*/
#include <stdio.h>	/* printf()				*/
#include <time.h>	/* time 				*/

#include "pipe-flower.h"

void pipe_flower_print(pipe_flower_t flower, const char *prefix)
{
	time_t timestamp = time(NULL);
	struct tm *info = localtime(&timestamp);
	/* YYYY-MM-DD HH:MM:SS: color: name */
	printf("%s: %04d-%02d-%02d %02d:%02d:%02d: %s: %s\n", prefix, info->tm_year + 1900, info->tm_mon + 1, info->tm_mday, info->tm_hour, info->tm_min, info->tm_sec, flower.name, flower.color);
}

int pipe_flower_set_color(pipe_flower_t *flower, const char *color)
{
	if (strlen(color) > PIPE_FLOWER_COLOR_SIZE - 1)
		return pipe_flower_error_color;
	else
		strcpy(flower->color, color);
	return 0;
}

int pipe_flower_set_name(pipe_flower_t *flower, const char *name)
{
	if (strlen(name) > PIPE_FLOWER_NAME_SIZE - 1)
		return pipe_flower_error_name;
	else
		strcpy(flower->name, name);
	return 0;
}