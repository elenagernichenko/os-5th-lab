#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Проверка количества аргументов
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <имя_файла> <N>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Открытие файла для чтения
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    // Преобразование второго аргумента в число
    int N = atoi(argv[2]);
    if (N < 0) {
        fprintf(stderr, "N должно быть неотрицательным числом.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int c;
    int line_count = 0; // Счетчик строк

    // Построчное чтение файла
    while ((c = fgetc(file)) != EOF) {
        putchar(c); // Вывод символа на экран

        // Подсчет строк
        if (c == '\n') {
            line_count++;
        }

        // Если N > 0 и достигнуто N строк, ждем нажатия клавиши
        if (N > 0 && line_count == N) {
            printf("\nНажмите любую клавишу для продолжения...\n");
            getchar(); // Ожидание нажатия клавиши
            line_count = 0; // Сброс счетчика строк
        }
    }

    // Закрытие файла
    if (fclose(file) == EOF) {
        perror("Ошибка закрытия файла");
        exit(EXIT_FAILURE);
    }

    printf("\nФайл успешно выведен.\n");
    return 0;
}

