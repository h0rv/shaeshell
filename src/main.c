#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cmds.c"

#define LINE_BUFFER_SIZE 1024
#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIMITERS " \t\r\n\a"

// #define PROMPT " //  "
// #define PROMPT " ●  "
// #define PROMPT " ○  "
#define PROMPT " •  "
// #define PROMPT " ▪  "
// #define PROMPT " ▫  "

void load_config() {}

char *get_line_buffer(int buffer_size) {
  return malloc(sizeof(char) * buffer_size);
}

char *increase_line_buffer_size(char *buffer, int new_buffer_size) {
  return realloc(buffer, sizeof(char) * new_buffer_size);
}

char *read_line() {
  int buffer_size = LINE_BUFFER_SIZE;
  char *line = get_line_buffer(buffer_size);

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
        fprintf(
            stderr,
            "shaeshell: allocation error: failed to increase line buffer size");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **get_token_buffer(int buffer_size) {
  return malloc(sizeof(char *) * buffer_size);
}

char **increase_token_buffer_size(char **buffer, int new_buffer_size) {
  return realloc(buffer, sizeof(char *) * new_buffer_size);
}

char **parse_line(char *line) {
  int buffer_size = TOKEN_BUFFER_SIZE;
  char **tokens = get_token_buffer(buffer_size);

  if (!tokens) {
    fprintf(stderr, "shaeshell: allocation error: failed to get tokens buffer");
    exit(EXIT_FAILURE);
  }

  int index = 0;

  char *token = strtok(line, TOKEN_DELIMITERS);
  while (token) {
    tokens[index] = token;
    index++;

    if (index >= buffer_size) {
      buffer_size += TOKEN_BUFFER_SIZE;
      tokens = increase_token_buffer_size(tokens, buffer_size);

      if (!tokens) {
        fprintf(stderr, "shaeshell: allocation error: failed to increase token "
                        "buffer size");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOKEN_DELIMITERS); // get next token
  }

  // terminate array
  tokens[index] = NULL;

  return tokens;
}

int launch(char **args) {

  int pid = fork();
  if (pid < 0) {

    // error
    perror("shaeshell: failed to fork process");

  } else if (pid == 0) {

    // child
    if (execvp(args[0], args) == -1) {
      perror("shaeshell: failed to exec child process");
    }
    exit(EXIT_FAILURE);

  } else {

    // parent
    int status;
    while (1) {
      int wpid = waitpid(pid, &status, WUNTRACED);
      if (WIFEXITED(status) || WIFSIGNALED(status))
        break;
    }
  }

  return 1;
}

int execute(char **args) {

  if (args[0] == NULL) {
    // empty command - ignore
    return 1;
  }

  for (int i = 0; i < num_cmds(); i++) {
    if (strcmp(args[0], cmds_str[i]) == 0) {
      return (*cmds_func[i])(args);
    }
  }

  return launch(args);
}

void loop() {
  char *line;
  char **args;
  int status = 1;

  while (status) {
    printf(PROMPT);

    line = read_line();
    args = parse_line(line);
    status = execute(args);

    free(line);
    free(args);
  }
}

void cleanup() {}

int main(int argc, char *argv[]) {

  load_config();
  loop();
  cleanup();

  return EXIT_SUCCESS;
}
