#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Проверка наличия аргумента (имени файла)
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <имя_файла>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Открытие файла для записи
    FILE *file = fopen(argv[1], "w");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    printf("Введите текст. Для выхода нажмите Ctrl-D:\n");

    int c;
    while ((c = getc(stdin)) != EOF) {
        // Запись символа в файл
        if (fputc(c, file) == EOF) {
            perror("Ошибка записи в файл");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    // Закрытие файла
    if (fclose(file) == EOF) {
        perror("Ошибка закрытия файла");
        exit(EXIT_FAILURE);
    }

    printf("\nФайл успешно записан.\n");
    return 0;
}


