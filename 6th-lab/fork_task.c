#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

// Функция для получения текущего времени в формате "часы:минуты:секунды:миллисекунды"
void print_current_time() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    struct tm *tm_info = localtime(&ts.tv_sec);
    printf("%02d:%02d:%02d:%03ld", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, ts.tv_nsec / 1000000);
}

int main() {
    pid_t pid1, pid2;

    // Первый вызов fork()
    pid1 = fork();

    if (pid1 == 0) {
        // Дочерний процесс 1
        printf("Дочерний процесс 1: PID = %d, PPID = %d, Время: ", getpid(), getppid());
        print_current_time();
        printf("\n");
    } else {
        // Родительский процесс
        // Второй вызов fork()
        pid2 = fork();

        if (pid2 == 0) {
            // Дочерний процесс 2
            printf("Дочерний процесс 2: PID = %d, PPID = %d, Время: ", getpid(), getppid());
            print_current_time();
            printf("\n");
        } else {
            // Родительский процесс
            sleep(1); // Даем время дочерним процессам завершиться
            printf("Родительский процесс: PID = %d, PPID = %d, Время: ", getpid(), getppid());
            print_current_time();
            printf("\n");

            // Выполняем команду ps -x
            system("ps -x");
        }
    }

    return 0;
}