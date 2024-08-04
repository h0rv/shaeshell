#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIMITERS " \t\r\n\a"

char** get_token_buffer(int buffer_size) { return malloc(sizeof(char*) * buffer_size); }

char** increase_token_buffer_size(char** buffer, int new_buffer_size) {
    return realloc(buffer, sizeof(char*) * new_buffer_size);
}

extern char** parse_line(char* line) {
    int buffer_size = TOKEN_BUFFER_SIZE;
    char** tokens = get_token_buffer(buffer_size);

    if (!tokens) {
        fprintf(stderr, "shaeshell: allocation error: failed to get tokens buffer");
        exit(EXIT_FAILURE);
    }

    int index = 0;

    char* token = strtok(line, TOKEN_DELIMITERS);
    while (token) {
        tokens[index] = token;
        index++;

        if (index >= buffer_size) {
            buffer_size += TOKEN_BUFFER_SIZE;
            tokens = increase_token_buffer_size(tokens, buffer_size);

            if (!tokens) {
                fprintf(stderr,
                        "shaeshell: allocation error: failed to increase token "
                        "buffer size");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIMITERS);  // get next token
    }

    // terminate array
    tokens[index] = NULL;

    return tokens;
}
