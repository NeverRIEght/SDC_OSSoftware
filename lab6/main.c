#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include <_stdlib.h>

#define FILENAME "output.txt"
#define NUM_PROCESSES 5

void getCurrentDateTimeZ(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%dT%H:%M:%S%z", tm_info);
}

int countGreetingsInFile(const char *greeting) {
    int fd = open(FILENAME, O_RDONLY);
    if (fd < 0) {
        perror("Error: can not open file for reading");
        return 0;
    }

    if (flock(fd, LOCK_SH) < 0) {
        perror("Error: can not lock file for reading");
        close(fd);
        return 0;
    }

    char buffer[1024];

    char currentLineBuffer[256];
    int lineIndex = 0;
    int greetingsCount = 0;

    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) { // While we have something to read
        for (ssize_t i = 0; i < bytesRead; i++) { // For each byte in buffer
            char currentChar = buffer[i];

            // If current char is end of line, or we reached the end of currentLineBuffer
            if (currentChar == '\n' || lineIndex >= (sizeof(currentLineBuffer) - 1)) {
                // Line is ended
                currentLineBuffer[lineIndex] = '\0';

                // If currentLineBuffer contains greeting
                if (strstr(currentLineBuffer, greeting) != NULL) {
                    greetingsCount++;
                }

                // Reset lineIndex to use it on next line
                lineIndex = 0;
            } else {
                // Add char to currentLineBuffer and proceed to next char
                currentLineBuffer[lineIndex++] = currentChar;
            }
        }
    }

    if (bytesRead < 0) {
        perror("Error: can not read file");
    }

    // If we did not reach the end of last line
    if (lineIndex > 0) {
        // Fix file ending
        currentLineBuffer[lineIndex] = '\0';
        if (strstr(currentLineBuffer, greeting) != NULL) {
            greetingsCount++;
        }
    }

    if (flock(fd, LOCK_UN) < 0) {
        perror("Error: can not unlock file for reading");
    }

    if (close(fd) < 0) {
        perror("Error: can not close file");
        exit(1);
    }

    return greetingsCount;
}

void writeGreetingToFile(const char *greeting, pid_t pid) {
    int fd = open(FILENAME, O_CREAT | O_RDWR | O_APPEND, 0644);
    if (fd < 0) {
        perror("Error: can not open file for writing");
        exit(1);
    }

    if (flock(fd, LOCK_EX) < 0) {
        perror("Error: can not lock file for writing");
        close(fd);
        exit(1);
    }
    
    char time_str[64];
    getCurrentDateTimeZ(time_str, sizeof(time_str));

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "PID: %d, Greeting: '%s', Time: %s\n",
             pid, greeting, time_str);

    if (write(fd, buffer, strlen(buffer)) < 0) {
        perror("Error: can not write to file");
    }

    if (flock(fd, LOCK_UN) < 0) {
        perror("Error: can not unlock file for writing");
    }

    if (close(fd) < 0) {
        perror("Error: can not close file");
        exit(1);
    }
}

int main() {
    const char *greetings[NUM_PROCESSES] = {
            "Hello!",
            "Hi!",
            "Hey!",
            "Salut!",
            "Welcome!"
    };

    pid_t pids[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Error forking");
            exit(1);
        } else if (pid == 0) {
            // Child process
            int count = countGreetingsInFile(greetings[i]);

            printf("PID: %d, Count in file: %d\n", getpid(), count);

            writeGreetingToFile(greetings[i], getpid());

            exit(0);
        } else {
            // Parent process
            pids[i] = pid;
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return 0;
}
