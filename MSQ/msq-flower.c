#include <stdlib.h> /* strlen()				*/
#include <string.h> /* strcmp(), strcpy()	*/
#include <stdio.h>	/* printf()				*/
#include <time.h>	/* time 				*/

#include "msq-flower.h"

void msq_flower_serialize(msq_flower_t flower, msq_flower_message_t *message)
{

	int i, j = 0;
	char text[MSQ_FLOWER_COLOR_SIZE + MSQ_FLOWER_NAME_SIZE];

	for (i = 0; flower.color[i] != '\0'; i++, j++)
	{
		text[j] = flower.color[i];
	}

	text[j] = '|';
	j++;

	for (i = 0; flower.name[i] != '\0'; i++, j++)
	{
		text[j] = flower.name[i];
	}

	text[j] = '\0';

	message->type = 1;
	strcpy(message->text, text);
}

void msq_flower_deserialize(msq_flower_t *flower, msq_flower_message_t message)
{

	char color[MSQ_FLOWER_COLOR_SIZE];
	char name[MSQ_FLOWER_NAME_SIZE];

	int i, j = 0;

	for (i = 0; message.text[j] != '|'; i++, j++)
	{
		color[i] = message.text[j];
	}

	color[i] = '\0';

	j++;

	for (i = 0; message.text[j] != '\0'; i++, j++)
	{
		name[i] = message.text[j];
	}

	name[i] = '\0';

	msq_flower_set_name(flower, name);
	msq_flower_set_color(flower, color);
}

void msq_flower_print(msq_flower_t flower)
{
	time_t timestamp = time(NULL);
	struct tm *info = localtime(&timestamp);
	/* YYYY-MM-DD HH:MM:SS: color: name */
	printf("%04d-%02d-%02d %02d:%02d:%02d: %s: %s\n", info->tm_year + 1900, info->tm_mon + 1, info->tm_mday, info->tm_hour, info->tm_min, info->tm_sec, flower.name, flower.color);
}

int msq_flower_set_color(msq_flower_t *flower, const char *color)
{
	if (strlen(color) > MSQ_FLOWER_COLOR_SIZE - 1)
		return msq_flower_error_color;
	else
		strcpy(flower->color, color);
	return 0;
}

int msq_flower_set_name(msq_flower_t *flower, const char *name)
{
	if (strlen(name) > MSQ_FLOWER_NAME_SIZE - 1)
		return msq_flower_error_name;
	else
		strcpy(flower->name, name);
	return 0;
}