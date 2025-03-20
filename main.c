#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "TypeInfo.h"
#include "utility.h"


void DisplayMenu(void) {
    printf("\n--- Меню операций с вектором ---\n");
    printf("1. Добавить элемент в конец\n");
    printf("2. Удалить элемент с конца\n");
    printf("3. Получить элемент по индексу\n");
    printf("4. Вставить элемент\n");
    printf("5. Удалить элемент по индексу\n");
    printf("6. Отсортировать вектор\n");
    printf("7. Применить функцию Map\n");
    printf("8. Применить функцию Where\n");
    printf("9. Вывести вектор\n");
    printf("10. Завершить программу\n");
    printf("Введите ваш выбор: ");
}

int main(void){
    TestCreate();
    TestCopy();
    TestPushBack();
    TestPopBack();
    TestGetElement();
    TestErase();
    TestInsert();
    TestWhere();
    TestMap();
    TestConcatenate();
    TestSort();

    
    
    Vector vector;
    Create(&vector, 4, type_info_int);
    int choice;
    int value, index;
    while (1) {
        DisplayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите целое число для добавления в конец: ");
                scanf("%d", &value);
                PushBack(&vector, &value);
                printf("Значение %d добавлено в конец.\n", value);
                break;

            case 2:
                if (vector.size > 0) {
                    PopBack(&vector);
                    printf("Последний элемент удален.\n");
                } else {
                    printf("Вектор пуст.\n");
                }
                break;

            case 3:
                printf("Введите индекс элемента для получения: ");
                scanf("%d", &index);
                int* element = (int*)GetElement(vector, index);
                if (element != NULL) {
                    printf("Элемент с индексом %d: %d\n", index, *element);
                } else {
                    printf("Неверный индекс.\n");
                }
                break;
            case 4:
                printf("Введите целое число для вставки: ");
                scanf("%d", &value);
                printf("Введите индекс для вставки: ");
                scanf("%d", &index);
                if (index < 0) {
                    printf("Индекс не может быть отрицательным. Значение %d добавлено в конец.\n", value);
                    PushBack(&vector, &value);
                } else if (index > vector.size) {
                    PushBack(&vector, &value);
                    printf("Индекс превышает размер вектора. Значение %d добавлено в конец.\n", value);
                } else {
                    Insert(&vector, &value, index);
                    printf("Значение %d вставлено по индексу %d.\n", value, index);
                }
                break;
            case 5:
                printf("Введите индекс для удаления: ");
                scanf("%d", &index);
                if (index < 0 || index >= vector.size) {
                    printf("Неверный индекс. Удаление невозможно.\n");
                } else {
                    Erase(&vector, index);
                    printf("Элемент с индексом %d удален.\n", index);
                }
                break;

            case 6:
                Sort(&vector);
                printf("Вектор отсортирован.\n");
                break;

            case 7:
                Map(&vector, IncrementInt);
                printf("Функция Map (инкремент) применена к вектору.\n");
                break;

            case 8:
                Where(&vector, IsEven);
                printf("Функция Where применена к вектору.\n");
                break;

            case 9:
                PrintVector(&vector);
                break;
            
            case 10:
                free(vector.data);
                printf("Выход из программы.\n");
                return 0;

            default:
                printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
        }
    }
    return 0;
}
    
    

