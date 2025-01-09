#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/file.h>

#define FILENAME "output.txt"
#define NUM_PROCESSES 5

void get_current_time(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%dT%H:%M:%S%z", tm_info);
}

int count_greetings_in_file(const char *greeting) {
    int fd = open(FILENAME, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file for reading");
        return 0; // Если файл не существует, считаем, что приветствий нет
    }

    char buffer[1024];
    ssize_t bytes_read;
    int count = 0;

    // Буфер для текущей строки
    char line[256];
    int line_index = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            char c = buffer[i];
            if (c == '\n' || line_index >= (sizeof(line) - 1)) {
                // Завершение строки
                line[line_index] = '\0';
                if (strstr(line, greeting) != NULL) {
                    count++;
                }
                line_index = 0;
            } else {
                // Добавляем символ к текущей строке
                line[line_index++] = c;
            }
        }
    }

    if (bytes_read < 0) {
        perror("Error reading file");
    }

    // Проверяем последнюю строку, если файл не заканчивается на '\n'
    if (line_index > 0) {
        line[line_index] = '\0';
        if (strstr(line, greeting) != NULL) {
            count++;
        }
    }

    close(fd);
    return count;
}

int main() {
    const char *greetings[NUM_PROCESSES] = {
            "Hello from process A!",
            "Greetings from process B!",
            "Hi there from process C!",
            "Welcome from process D!",
            "Hey from process E!"
    };

    int fd = open(FILENAME, O_CREAT | O_RDWR | O_APPEND, 0644);
    if (fd < 0) {
        perror("Error opening file");
        exit(1);
    }

    pid_t pids[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Error forking");
            exit(1);
        } else if (pid == 0) {
            flock(fd, LOCK_EX);

            char time_str[32];
            get_current_time(time_str, sizeof(time_str));

            int count = count_greetings_in_file(greetings[i]);

            char output[256];
            snprintf(output, sizeof(output), "PID: %d, Greeting: '%s', Time: %s\n",
                     getpid(), greetings[i], time_str);
            write(fd, output, strlen(output));

            printf("Process %d says: '%s' (Launch count: %d)\n", getpid(), greetings[i], count + 1);

            flock(fd, LOCK_UN);
            close(fd);
            exit(0);
        } else {
            pids[i] = pid;
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        waitpid(pids[i], NULL, 0);
    }

    close(fd);
    return 0;
}
