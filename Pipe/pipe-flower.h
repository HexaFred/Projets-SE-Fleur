/**
 * \file pipe-flower.h
 */
#ifndef PIPE_FLOWER_H
#define PIPE_FLOWER_H
/**
 * The size of the flower color.
 */
#define PIPE_FLOWER_COLOR_SIZE 20
/**
 * The size of the flower name.
 */
#define PIPE_FLOWER_NAME_SIZE 15
/**
 * A flower.
 */
typedef struct
{
    char color[PIPE_FLOWER_COLOR_SIZE]; /**< The flower color (example: "red"). */
    char name[PIPE_FLOWER_NAME_SIZE]; /**< The flower name (example: "rose"). */
} pipe_flower_t;
/**
 * A flower error
 */
typedef enum
{
    pipe_flower_error_color = -2,
    pipe_flower_error_name
} pipe_flower_error_t;
/**
 * Prints a flower using the "prefix: YYYY-MM-DD HH:MM:SS: name: color" format.
 * \param flower The flower.
 * \param prefix The prefix.
 */
void pipe_flower_print(pipe_flower_t flower, const char *prefix);
/**
 * Sets the color of a flower.
 * \param flower The flower.
 * \param color the color.
 * \return pipe_flower_error_color if the flowze color length is greater than PIPE_FLOWER_COLOR_SIZE - 1,else 0.
 */
int pipe_flower_set_color(pipe_flower_t *flower, const char *color);
/**
 * Sets the name of a flower.
 * \param flower The flower.
 * \param name The name.
 * \return pipe_flower_error_name if the flower name length is greater than PIPE_FLOWER_NAME_SIZE - 1, else 0.
 */
int pipe_flower_set_name(pipe_flower_t *flower, const char *name);
#endif