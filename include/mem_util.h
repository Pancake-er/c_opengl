#ifndef MEM_UTIL_INCLUDED
#define MEM_UTIL_INCLUDED

#include <stdio.h>

void *malloc_or_die(size_t size);
void *calloc_or_die(size_t number_of_elements, size_t size_of_one_element);
size_t get_line_delim(char **line, size_t *buffer_size, int delim, FILE *file);

#endif