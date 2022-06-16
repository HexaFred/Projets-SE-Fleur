/**
 * \file shm-common.h
 */
#ifndef SHM_COMMON_H
#define SHM_COMMON_H
/**
 * convert a pathname and a project identifier to a System V IPC key
 */
int create_key(char* pathname, int proj_id, key_t *key);
/**
 * Print the program pathname and proj_id.
 */
void print_settings(char* pathname, int proj_id);
#endif