#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <_stdlib.h>
#include <signal.h>

void printPId(int process_number) {
    while (1) {
        printf("Subprocess %d (PID: %d) is running...\n", process_number, getpid());
        sleep(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <number_of_processes> <exec_args...>\n", argv[0]);
        exit(1);
    }

    // processes count
    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        fprintf(stderr, "Number of processes must be greater than 0.\n");
        exit(1);
    }

    // exec args
    char **exec_args = &argv[2];

    pid_t *pid = malloc(num_processes * sizeof(pid_t));
    if (pid == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    int i;

    for (i = 0; i < num_processes; i++) {
        pid[i] = fork();

        if (pid[i] < 0) {
            perror("Fork failed");
            exit(1);
        }

        if (pid[i] == 0) {
            // Subprocesses code
            if (i == 2) {
                execvp(exec_args[0], exec_args);
                perror("Exec failed");
                exit(1);
            } else {
                printPId(i + 1);
            }
        }
    }

    // Parent code
    printf("Parent process (PID: %d) is waiting...\n", getpid());
    sleep(5);

    printf("Parent process is terminating all child processes...\n");
    for (i = 0; i < 4; i++) {
        kill(pid[i], SIGKILL);
        printf("Terminated process with PID: %d\n", pid[i]);
    }


    for (i = 0; i < 4; i++) {
        waitpid(pid[i], NULL, 0);
    }

    printf("All child processes are terminated. Parent process exiting...\n");

    free(pid);
    return 0;
}