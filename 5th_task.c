#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // Для копирования прав доступа

int main(int argc, char *argv[]) {
    // Проверка количества аргументов
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <исходный_файл> <целевой_файл>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Открытие исходного файла для чтения
    FILE *src_file = fopen(argv[1], "r");
    if (src_file == NULL) {
        perror("Ошибка открытия исходного файла");
        exit(EXIT_FAILURE);
    }

    // Открытие целевого файла для записи
    FILE *dst_file = fopen(argv[2], "w");
    if (dst_file == NULL) {
        perror("Ошибка открытия целевого файла");
        fclose(src_file);
        exit(EXIT_FAILURE);
    }

    // Копирование содержимого файла посимвольно
    int c;
    while ((c = fgetc(src_file)) != EOF) {
        if (fputc(c, dst_file) == EOF) {
            perror("Ошибка записи в целевой файл");
            fclose(src_file);
            fclose(dst_file);
            exit(EXIT_FAILURE);
        }
    }

    // Закрытие файлов
    if (fclose(src_file) == EOF) {
        perror("Ошибка закрытия исходного файла");
        fclose(dst_file);
        exit(EXIT_FAILURE);
    }
    if (fclose(dst_file) == EOF) {
        perror("Ошибка закрытия целевого файла");
        exit(EXIT_FAILURE);
    }

    // Копирование прав доступа
    struct stat st;
    if (stat(argv[1], &st) == -1) {
        perror("Ошибка получения прав доступа исходного файла");
        exit(EXIT_FAILURE);
    }
    if (chmod(argv[2], st.st_mode) == -1) {
        perror("Ошибка установки прав доступа целевого файла");
        exit(EXIT_FAILURE);
    }

    printf("Файл успешно скопирован с сохранением прав доступа.\n");
    return 0;
}