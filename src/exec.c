#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cmds.c"

int launch(char** args) {
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
            if (WIFEXITED(status) || WIFSIGNALED(status)) break;
        }
    }

    return 1;
}

extern int execute(char** args) {
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
