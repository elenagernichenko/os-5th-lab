#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// Функция для поиска файлов в каталоге и подкаталогах
void search_files(const char *dir_path, long min_size, long max_size, FILE *output_file, int *file_count) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Ошибка открытия каталога");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Пропустить текущий и родительский каталоги
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == -1) {
            perror("Ошибка получения информации о файле");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            // Если это каталог, рекурсивно ищем в нем
            search_files(full_path, min_size, max_size, output_file, file_count);
        } else if (S_ISREG(st.st_mode)) {
            // Если это файл, проверяем его размер
            (*file_count)++;
            if (st.st_size >= min_size && st.st_size <= max_size) {
                fprintf(output_file, "%s, %s, %ld\n", full_path, entry->d_name, st.st_size);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Использование: %s <каталог> <min_size> <max_size> <выходной_файл>\n", argv[0]);
        return 1;
    }

    const char *dir_path = argv[1];
    long min_size = atol(argv[2]);
    long max_size = atol(argv[3]);
    const char *output_file_name = argv[4];

    FILE *output_file = fopen(output_file_name, "w");
    if (!output_file) {
        perror("Ошибка открытия выходного файла");
        return 1;
    }

    int file_count = 0;
    search_files(dir_path, min_size, max_size, output_file, &file_count);

    fclose(output_file);
    printf("Общее число просмотренных файлов: %d\n", file_count);

    return 0;
}