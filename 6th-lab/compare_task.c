#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_PATH 1024

// Функция для сравнения содержимого двух файлов
int compare_files(const char *file1, const char *file2, int *are_identical) {
    int fd1 = open(file1, O_RDONLY), fd2 = open(file2, O_RDONLY);
    if (fd1 == -1 || fd2 == -1) return -1; // Ошибка открытия файлов

    char buf1[1024], buf2[1024];
    ssize_t bytes1, bytes2;
    size_t total_bytes = 0;
    *are_identical = 1; // Предполагаем, что файлы идентичны

    // Построчное чтение и сравнение файлов
    while ((bytes1 = read(fd1, buf1, sizeof(buf1))) > 0) {
        bytes2 = read(fd2, buf2, bytes1);
        total_bytes += bytes1;

        if (bytes2 != bytes1 || memcmp(buf1, buf2, bytes1)) {
            *are_identical = 0; // Файлы различны
        }
    }

    close(fd1); close(fd2);
    return total_bytes; // Возвращаем количество прочитанных байт
}

int main() {
    char dir1[MAX_PATH], dir2[MAX_PATH];
    int N;

    // Ввод директорий и максимального числа процессов
    printf("Введите Dir1: "); scanf("%s", dir1);
    printf("Введите Dir2: "); scanf("%s", dir2);
    printf("Введите максимальное число процессов: "); scanf("%d", &N);

    // Открытие директорий
    DIR *dp1 = opendir(dir1), *dp2 = opendir(dir2);
    if (!dp1 || !dp2) { perror("Ошибка открытия директории"); exit(1); }

    struct dirent *entry1, *entry2;
    char path1[MAX_PATH], path2[MAX_PATH];
    int active_processes = 0;

    // Чтение файлов из Dir1
    while ((entry1 = readdir(dp1)) != NULL) {
        if (entry1->d_type != DT_REG) continue; // Пропуск нерегулярных файлов

        // Формирование полного пути к файлу в Dir1
        snprintf(path1, MAX_PATH, "%.*s/%.*s",
                 (int)(MAX_PATH - strlen(entry1->d_name) - 2), dir1,
                 (int)MAX_PATH, entry1->d_name);

        // Чтение файлов из Dir2
        while ((entry2 = readdir(dp2)) != NULL) {
            if (entry2->d_type != DT_REG) continue;

            // Формирование полного пути к файлу в Dir2
            snprintf(path2, MAX_PATH, "%.*s/%.*s",
                     (int)(MAX_PATH - strlen(entry2->d_name) - 2), dir2,
                     (int)MAX_PATH, entry2->d_name);

            // Ограничение на количество одновременно работающих процессов
            if (active_processes >= N) {
                wait(NULL); // Ожидание завершения одного из процессов
                active_processes--;
            }

            // Создание дочернего процесса для сравнения файлов
            if (fork() == 0) {
                int are_identical;
                int result = compare_files(path1, path2, &are_identical);
                if (result == -1) {
                    printf("PID: %d, Файлы: %s и %s, Ошибка открытия файлов\n",
                           getpid(), entry1->d_name, entry2->d_name);
                } else if (are_identical) {
                    printf("PID: %d, Файлы: %s и %s, Сравнено байт: %d, Файлы идентичны\n",
                           getpid(), entry1->d_name, entry2->d_name, result);
                } else {
                    printf("PID: %d, Файлы: %s и %s, Сравнено байт: %d, Файлы различны\n",
                           getpid(), entry1->d_name, entry2->d_name, result);
                }
                exit(0); // Завершение дочернего процесса
            } else {
                active_processes++; // Увеличение счетчика активных процессов
            }
        }
        rewinddir(dp2); // Сброс указателя директории для следующего файла
    }

    // Ожидание завершения всех дочерних процессов
    while (active_processes > 0) {
        wait(NULL);
        active_processes--;
    }

    // Закрытие директорий
    closedir(dp1); closedir(dp2);
    return 0;
}