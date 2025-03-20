#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compare.h"
#include "destruct.h"

void SetElement(Vector* vector, const void* data) {
    char* dest = (char*)vector->data + vector->size * vector->type_info.size_element;
    memcpy(dest, data, vector->type_info.size_element);
}

void Create(Vector* vector, size_t capacity, TypeInfo type_info) {
    vector->data = (capacity == 0) ? NULL : calloc(capacity, type_info.size_element);
    vector->size = 0;
    vector->capacity = capacity;
    vector->type_info = type_info;
}

void Clear(Vector* vector) {
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

static void* MyRealoc(void* data, size_t old_capacity, size_t new_capacity, size_t size_element) {
    void* new_data = calloc(new_capacity, size_element);
    if (data && old_capacity > 0) {
        memcpy(new_data, data, old_capacity * size_element);
    }
    free(data);
    return new_data;
}

void PushBack(Vector* vector, const void* value) {
    if (vector->size == vector->capacity) {
        size_t new_capacity = (vector->capacity == 0) ? 1 : vector->capacity * 2;
        vector->data = MyRealoc(vector->data, vector->capacity, new_capacity, vector->type_info.size_element);
        vector->capacity = new_capacity;
    }
    SetElement(vector, value);
    vector->size++;
}

int PopBack(Vector* vector) {
    if (vector->size == 0) return 0;
    vector->size--;
    if (vector->type_info.destruct) {
        vector->type_info.destruct((char*)vector->data + vector->size * vector->type_info.size_element);
    }
    return 1;
}

void* GetElement(Vector vector, size_t index) {
    if (index >= vector.size) return NULL;
    return (char*)vector.data + index * vector.type_info.size_element;
}

int Insert(Vector* vector, const void* value, size_t index) {
    if (index > vector->size) return 1;

    if (vector->size == vector->capacity) {
        size_t new_capacity = (vector->capacity == 0) ? 1 : vector->capacity * 2;
        void* new_data = MyRealoc(vector->data, vector->capacity, new_capacity, vector->type_info.size_element);
        if (!new_data) return 1;
        vector->data = new_data;
        vector->capacity = new_capacity;
    }

    char* data_ptr = (char*)vector->data;
    size_t element_size = vector->type_info.size_element;

    for (size_t i = vector->size; i > index; --i) {
        memmove(
            data_ptr + i * element_size,
            data_ptr + (i - 1) * element_size,
            element_size
        );
    }
    memcpy(data_ptr + index * element_size, value, element_size);
    vector->size++;
    return 0;
}

int Erase(Vector* vector, size_t index) {
    if (index >= vector->size || !vector->data) {
        return 1;
    }
    char* data_ptr = (char*)vector->data;
    size_t element_size = vector->type_info.size_element;
    for (size_t i = index; i < vector->size - 1; ++i) {
        memmove(
            data_ptr + i * element_size,
            data_ptr + (i + 1) * element_size,
            element_size
        );
    }

    vector->size--;
    return 0;
}

Vector Copy(const Vector* other) {
    Vector vector;
    Create(&vector, other->capacity, other->type_info);
    memcpy(vector.data, other->data, other->size * other->type_info.size_element);
    vector.size = other->size;
    return vector;
}

Vector Where(const Vector* vector, int (*Predicate)(void*)) {
    Vector result;
    Create(&result, 0, vector->type_info);
    char* data_ptr = (char*)vector->data;
    for (size_t i = 0; i < vector->size; ++i) {
        void* element = data_ptr + i * vector->type_info.size_element;
        if (Predicate(element)) {
            PushBack(&result, element);
        }
    }
    return result;
}

Vector Map(Vector* vector, void (*unary)(void*)) {
    Vector result = Copy(vector);
    char* data_ptr = (char*)result.data;
    for (size_t i = 0; i < result.size; ++i) {
        unary(data_ptr + i * result.type_info.size_element);
    }
    return result;
}

Vector Concatenate(const Vector* vector1, const Vector* vector2) {
    Vector result;
    Create(&result, vector1->size + vector2->size, vector1->type_info);
    memcpy(result.data, vector1->data, vector1->size * vector1->type_info.size_element);
    memcpy(
        (char*)result.data + vector1->size * vector1->type_info.size_element,
        vector2->data,
        vector2->size * vector2->type_info.size_element
    );
    result.size = vector1->size + vector2->size;
    return result;
}

void Merge(void* data, size_t first_size, size_t second_size, void* buffer, size_t size_element, int (*compare)(const void*, const void*)){
    char* copy_data = data;
    char* copy_byfer = buffer;
    size_t i = 0;
    size_t j = 0;
    while(i < first_size && j < second_size){
        if(compare(copy_data + i * size_element, copy_data + (first_size + j) * size_element) > 0){
            memcpy(copy_byfer + (i + j) * size_element, copy_data + (first_size + j) * size_element, size_element);
            ++j;
        }else{
            memcpy(copy_byfer + (i + j) * size_element, copy_data + i * size_element, size_element);
            ++i;
        }
    }
    while (i < first_size) {
        memcpy(copy_byfer + (i + j) * size_element, copy_data + i * size_element, size_element);
        ++i;
    }
    while (j < second_size) {
        memcpy(copy_byfer + (i + j) * size_element, copy_data + (first_size + j) * size_element, size_element);
        ++j;
    }
    memcpy(data, copy_byfer, (i + j) * size_element);
}

void MergeSortRec(void* data, size_t size, size_t size_element, int (*compare)(const void*, const void*), void* buffer){
    if(size < 2){
        return;
    }
    char* copy_data = data;
    MergeSortRec(data, size/2, size_element, compare, buffer);
    MergeSortRec(copy_data + (size/2) * size_element, size - size /2 , size_element, compare, buffer);
    Merge(data, size/2, size - size/2, buffer, size_element, compare);
}

void Sort(Vector* vector){
    if (vector->type_info.compare == NULL) {
        return;
    }
    char* buffer = calloc(vector->size, vector->type_info.size_element);
    MergeSortRec(vector->data, vector->size, vector->type_info.size_element, vector->type_info.compare, buffer);
    free(buffer);
}

void PrintVector(const Vector* vector) {
    if (vector->data == NULL) {
        printf("Вектор пустой или не инициализирован.\n");
        return;
    }

    printf("Вектор: ");
    for (size_t i = 0; i < vector->size; ++i) {
        void* element = GetElement(*vector, i);
        if (vector->type_info.compare == IntCompare) {
            printf("%d ", *(int*)element);
        } else if (vector->type_info.compare == DoubleCompare) {
            printf("%.2f ", *(double*)element);
        }else {
            printf("Неизвестный тип.\n");
            return;
        }
    }
    printf("\n");
}
