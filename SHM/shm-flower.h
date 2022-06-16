/**
 * \file shm-flower.h
 */
#ifndef SHM_FLOWER_H
#define SHM_FLOWER_H
/**
 * The size of the flower color.
 */
#define SHM_FLOWER_COLOR_SIZE 20
/**
 * The size of the flower name.
 */
#define SHM_FLOWER_NAME_SIZE 15
/**
 * A flower.
 */
typedef struct 
{
    char color[SHM_FLOWER_COLOR_SIZE]; /**< The flower color (example: "red"). */
    char name[SHM_FLOWER_NAME_SIZE]; /**< The flower name (example: "rose"). */
} shm_flower_t;
/**
 * A flower error.
 */
typedef enum
{
    shm_flower_error_color = -3,
    shm_flower_error_name,
    shm_flower_error_not_empty
} shm_flower_error_t;
/**
 * Copies a source flower to a target flower
 * \param source The source flower.
 * \param target The target flower.
 * \return shm_flower_error_not_empty if the target flower is not empty, shm_flower_error_color if the shm_flower_set_color function returns shm_flower_error_color, shm_flower_error_name if the shm_flower_set_name function returns shm_flower_error_name, else 0.
 */
int shm_flower_copy(shm_flower_t source, shm_flower_t *target);
/**
 * Empties a flower (i.e., sets the flower color and name to an empty string).
 * \param flower The flower.
 */
void shm_flower_empty(shm_flower_t *flower);
/**
 * Checks if a flower is empty (i.e., checks if the flower color and name are an empty string).
 * \param flower The flower.
 * \return 1 if the flower is empty, else 0.
 */
int shm_flower_is_empty(shm_flower_t flower);
/**
 * Prints a flower using the "YYYY-MM-DD HH:MM:SS: name: color" format.
 * \param flower The flower.
 */
void shm_flower_print(shm_flower_t flower);
/**
 * Sets the color of a flower.
 * \param flower The flower.
 * \param color The color.
 * \return shm_flower_error_color if the flower color length is greater than SHM_FLOWER_COLOR_SIZE -1, else 0.
 */
int shm_flower_set_color(shm_flower_t *flower,const char *color);
/**
 * Sets the name of a flower.
 * \param flower The flower.
 * \param name The name.
 * \return shm_flower_error_name if the flower name length is greater than SHM_FLOWER_NAME_SIZE -1, else 0.
 */
int shm_flower_set_name(shm_flower_t *flower,const char *name);
#endif