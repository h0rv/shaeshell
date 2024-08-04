#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Built-in commands
int cd(char **args);
int help(char **args);
int exit_shaeshell(char **args);
int num_cmds();

extern char *cmds_str[];
extern int (*cmds_func[])(char **);

char *cmds_str[] = {
    "cd",
    "help",
    "exit",
};

int (*cmds_func[])(char **) = {
    &cd,
    &help,
    &exit_shaeshell,
};

int num_cmds() { return sizeof(cmds_str) / sizeof(char *); }

int cd(char **args) {
  char *dir = args[1];

  if (dir == NULL) {
    dir = getenv("HOME");
  }

  int status = chdir(dir);

  if (status != 0) {
    perror("shaeshell: failed to cd");
  }

  return 1;
}

int help(char **args) {
  printf("h0rv's shaeshell\n\n");
  printf("Supported built-in commands:\n");
  for (int i = 0; i < num_cmds(); i++) {
    printf("  %s\n", cmds_str[i]);
  }
  return 1;
}

int exit_shaeshell(char **args) { return 0; }
