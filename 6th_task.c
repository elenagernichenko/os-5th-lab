#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> // Для работы с каталогами
#include <errno.h>  // Для обработки ошибок

// Функция для вывода содержимого каталога
void print_directory_contents(const char *dir_name) {
    DIR *dir = opendir(dir_name); // Открытие каталога
    if (dir == NULL) {
        perror("Ошибка открытия каталога");
        return;
    }

    printf("Содержимое каталога '%s':\n", dir_name);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) { // Чтение записей каталога
        printf("%s\n", entry->d_name); // Вывод имени файла/каталога
    }

    if (errno != 0) { // Проверка на ошибки при чтении
        perror("Ошибка чтения каталога");
    }

    if (closedir(dir) == -1) { // Закрытие каталога
        perror("Ошибка закрытия каталога");
    }
}

int main(int argc, char *argv[]) {
    // Вывод содержимого текущего каталога
    printf("Текущий каталог:\n");
    print_directory_contents(".");

    // Вывод содержимого каталога, переданного в качестве аргумента
    if (argc > 1) {
        printf("\nКаталог '%s':\n", argv[1]);
        print_directory_contents(argv[1]);
    } else {
        printf("\nДополнительный каталог не указан.\n");
    }

    return 0;
}