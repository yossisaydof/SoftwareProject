/*
 * error_handler Summary:
 *
 * Used for returning error codes from any module.
 */

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdio.h>
#include <stdlib.h>

#define DIVIDE_BY_ZERO "Error: divide by zero\n"
#define FILE_WRITING "Error: writing to file failed\n"
#define FILE_READING "Error: reading from file failed\n"
#define FILE_OPENING "Error: opening file failed\n"
#define FILE_SEEK "Error: failed to seek file\n"
#define MALLOC_FAILED "Error: malloc failed\n"

#define ERROR_HANDLER(error) {printf("%s", error); exit(EXIT_FAILURE);}

#endif
