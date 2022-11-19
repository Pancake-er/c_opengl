#include <stdio.h>
#include <stdlib.h>

void *malloc_or_die(size_t size)
{
    void *alloc_mem = malloc(size);

    if(alloc_mem == NULL)
    {
        perror("Failed to allocate memory");
        free(alloc_mem);
        exit(EXIT_FAILURE);
    }
    return alloc_mem;
}
void *calloc_or_die(size_t number_of_elements, size_t size_of_one_element)
{
    void *alloc_mem = calloc(number_of_elements, size_of_one_element);

    if(alloc_mem == NULL)
    {
        perror("Failed to allocate memory");
        free(alloc_mem);
        exit(EXIT_FAILURE);
    }
    return alloc_mem;
}
size_t get_line_delim(char **line, size_t *buffer_size, int delim, FILE *file) {
    int file_char;
    size_t line_length = 0;
    while((file_char = fgetc(file)) != EOF) {
        if(line_length + 1 >= *buffer_size) {
            char *temp_line_ptr = realloc(*line, *buffer_size + 128);
            if (temp_line_ptr == NULL) {
                perror("Failed to reallocate memory");
                free(temp_line_ptr);
                exit(EXIT_FAILURE);
            }
            *buffer_size += 128;
            *line = temp_line_ptr;
        }
        (*line)[line_length++] = file_char;
        if(file_char == delim) {
            break;
        }
    }
    (*line)[line_length] = '\0';
    return line_length;
}