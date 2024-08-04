#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.c"
#include "exec.c"
#include "parse.c"
#include "read.c"

// #define PROMPT " //  "
// #define PROMPT " ●  "
// #define PROMPT " ○  "
#define PROMPT " •  "
// #define PROMPT " ▪  "
// #define PROMPT " ▫  "

void print_prompt() { printf(PROMPT); }

void loop() {
    int status = 1;
    while (status) {
        print_prompt();

        char* line = read_line();
        char** args = parse_line(line);
        status = execute(args);

        free(line);
        free(args);
    }
}

void cleanup() {}

int main(int argc, char* argv[]) {
    load_config();
    loop();
    cleanup();

    return EXIT_SUCCESS;
}
