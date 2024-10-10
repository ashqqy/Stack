/// @file

#include "MyAllocation.h"

#include <stdlib.h>
#include <assert.h>

//-------------------------------------------------------

/// @brief Функция выделения блока памяти
/// @param n_elems Новое количество элементов для выделения
/// @param size_elems Размер элементов для выделения
/// @param poison Указатель на ядовитое значение, которым заполнятся выделенные ячейки
/// @return Указатель на выделенный блок памяти

void* MyCalloc (size_t n_elems, size_t size_elems, void* poison)
    {
    void* mem_ptr = calloc (n_elems, size_elems);
    for (int i = 0; i < n_elems; i++)
        memcpy ((char*) mem_ptr + i * size_elems, poison, size_elems);
    return mem_ptr;
    }

//-------------------------------------------------------

/// @brief Функция изменения размера уже выделенного блока памяти
/// @param[in] memory Указатель на выделенный блок памяти
/// @param[in] n_elements Новое количество элементов для выделения
/// @param[in] size_elements Размер элементов для выделения
/// @param[in] previous_n_elements Количество элементов в блоке до вызова функции
/// @param[in] poison Указатель на ядовитое значение, которым заполнятся добавленные ячейки
/// @return Указатель на перевыделенный блок памяти

void* MyRecalloc (void* memory, size_t n_elements, size_t size_elements, size_t previous_n_elements, void* poison)
    {
    void* save_memory = memory;
    memory = realloc (memory, n_elements * size_elements);
    /// Если память не удалось перевыделить, то освобождаем старый указатель и возвращаем 0
    if (memory == NULL)
        {
        free (save_memory); save_memory = NULL;
        return NULL;
        }

    /// Если увеличиваем блок памяти, то новые ячейки заполняем ядовитым значением
    if (previous_n_elements < n_elements)
        for (int i = 0; i < n_elements - previous_n_elements; i++)
            memcpy(((char*) memory + (previous_n_elements + i) * size_elements), poison, size_elements);

    /// Если уменьшаем блок памяти, то удаленные ячейки обнуляем
    if (previous_n_elements > n_elements)
        for (int i = 0; i < previous_n_elements - n_elements; i++)
            memset ((char*) memory + n_elements * size_elements, 0, size_elements);

    return memory;
    }

//-------------------------------------------------------