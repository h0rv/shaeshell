#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define LINE_BUFFER_SIZE 1024

char* get_line_buffer(int buffer_size) { return malloc(sizeof(char) * buffer_size); }

char* increase_line_buffer_size(char* buffer, int new_buffer_size) {
    return realloc(buffer, sizeof(char) * new_buffer_size);
}

extern char* read_line() {
    int buffer_size = LINE_BUFFER_SIZE;
    char* line = get_line_buffer(buffer_size);

    if (!line) {
        fprintf(stderr, "shaeshell: allocation error: failed to get line buffer");
        exit(EXIT_FAILURE);
    }

    int index = 0;

    while (1) {
        int curr_char = getchar();

        // EOF or newline signals we are done;
        // terminate line string and return
        if (curr_char == EOF || curr_char == '\n') {
            line[index] = '\0';
            return line;
        }

        line[index] = curr_char;
        index++;

        if (index >= buffer_size) {
            buffer_size += LINE_BUFFER_SIZE;
            line = increase_line_buffer_size(line, buffer_size);

            if (!line) {
                fprintf(stderr,
                        "shaeshell: allocation error: failed to "
                        "increase line buffer size");
                exit(EXIT_FAILURE);
            }
        }
    }
}
