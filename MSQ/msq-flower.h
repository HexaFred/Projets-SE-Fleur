/**
 * \file MSQ-flower.h
 */
#ifndef MSQ_FLOWER_H
#define MSQ_FLOWER_H
/**
 * The size of the flower color.
 */
#define MSQ_FLOWER_COLOR_SIZE 20
/**
 * The size of the flower name.
 */
#define MSQ_FLOWER_NAME_SIZE 15
/**
 * A flower.
 */
typedef struct
{
    char color[MSQ_FLOWER_COLOR_SIZE]; /**< The flower color (example: "red"). */
    char name[MSQ_FLOWER_NAME_SIZE];   /**< The flower name (example: "rose"). */
} msq_flower_t;
/**
 * A message.
 */
typedef struct
{
    long type;                                               /**< The message type (example: "1"). */
    char text[MSQ_FLOWER_COLOR_SIZE + MSQ_FLOWER_NAME_SIZE]; /**< The message text (example: "red|rose"). */
} msq_flower_message_t;
/**
 * A flower error.
 */
typedef enum
{
    msq_flower_error_color = -2,
    msq_flower_error_name
} msq_flower_error_t;
/**
 * Serializes a flower to a message (the message type is "1" and the message text is using the "color|name" format).
 * \param flower The flower.
 * \param message The message.
 */
void msq_flower_serialize(msq_flower_t flower, msq_flower_message_t *message);
/**
 * Deserializes a message to a flower (the message type is "1" and the message text is using the "color|name" format).
 * \param flower The flower.
 * \param message The message.
 */
void msq_flower_deserialize(msq_flower_t *flower, msq_flower_message_t message);
/**
 * Prints a flower using the "YYYY-MM-DD HH:MM:SS: name: color" format.
 * \param flower The flower.
 */
void msq_flower_print(msq_flower_t flower);
/**
 * Sets the color of a flower.
 * \param flower The flower.
 * \param color The color.
 * \return msq_flower_error_color if the flower color length is greater than MSQ_FLOWER_COLOR_SIZE -1, else 0.
 */
int msq_flower_set_color(msq_flower_t *flower, const char *color);
/**
 * Sets the name of a flower.
 * \param flower The flower.
 * \param name The name.
 * \return msq_flower_error_name if the flower name length is greater than MSQ_FLOWER_NAME_SIZE -1, else 0.
 */
int msq_flower_set_name(msq_flower_t *flower, const char *name);
#endif