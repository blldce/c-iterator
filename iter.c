

#include <stdio.h>
#include <stdlib.h>
#include "iter.h"


struct iter *iter_init_stack_depend(int(*buffer_ptr), int arr_size)
{
    struct iter(*iter_instance) = (struct iter(*))malloc(sizeof(struct iter));
    iter_instance->next = next;
    iter_instance->filter = filter;
    iter_instance->map = map;
    iter_instance->buffer_size = arr_size;
    iter_instance->buffer_ptr_iter = buffer_ptr;
    iter_instance->buffer_start = buffer_ptr;
    iter_instance->buffer_end = buffer_ptr + (sizeof(char) * arr_size - 1);
    return iter_instance;
}

struct iter *iter_init_seperate(int(*buffer_ptr), int arr_size)
{
    struct iter(*iter_instance) = (struct iter(*))malloc(sizeof(struct iter) + arr_size);
    iter_instance->next = next;
    iter_instance->filter = filter;
    iter_instance->map = map;
    iter_instance->buffer_size = arr_size;
    
    int count = 0;
    while(count < arr_size)
    {
        iter_instance->buffer[count] = buffer_ptr[count];
        count++;
    }
    iter_instance->buffer_ptr_iter = iter_instance->buffer;
    iter_instance->buffer_start = iter_instance->buffer;
    iter_instance->buffer_end = iter_instance->buffer + (sizeof(char) * arr_size - 1);
    return iter_instance;
}

int next(struct iter(*iter_ptr))
{
    if (iter_ptr->buffer_ptr_iter > iter_ptr->buffer_end)
        return 0; // we're in end item, no left!
    return *(iter_ptr->buffer_ptr_iter++);
}

void map(struct iter(*iter_ptr), int (*func_ptr)(int))
{
    // reset pointer to first
    iter_ptr->buffer_ptr_iter = iter_ptr->buffer_start;

    while (iter_ptr->buffer_ptr_iter <= iter_ptr->buffer_end)
    {
        *iter_ptr->buffer_ptr_iter = func_ptr(*iter_ptr->buffer_ptr_iter);
        iter_ptr->buffer_ptr_iter++;
    }
    // reset pointer to first
    iter_ptr->buffer_ptr_iter = iter_ptr->buffer_start;
}

void filter(struct iter(*iter_ptr), int (*func_ptr)(int))
{
    // reset pointer to first
    iter_ptr->buffer_ptr_iter = iter_ptr->buffer_start;

    int buff_size = 0;
    int count = 0;
    int(*temp_buff) = (int(*))malloc(sizeof(int) * iter_ptr->buffer_size); // at most buffer_size len (if each item filtered)
    // iterate each filter & store filtered in temp buffer
    while (iter_ptr->buffer_ptr_iter <= iter_ptr->buffer_end)
    {
        if (func_ptr(*iter_ptr->buffer_ptr_iter) != 0)
        {
            temp_buff[buff_size] = *iter_ptr->buffer_ptr_iter;
            buff_size++;
        }
        count++;
        iter_ptr->buffer_ptr_iter++;
    }
    if (buff_size == 0)
    {
        free(temp_buff);
        return;
    }

    int(*new_buffer) = (int(*))malloc(buff_size);

    // apply each filtered item to new buffer
    count = 0;
    while (count < buff_size)
    {
        new_buffer[count] = temp_buff[count];
        count++;
    }
    free(temp_buff);
    iter_ptr->buffer_size = buff_size;
    iter_ptr->buffer_ptr_iter = new_buffer;
    iter_ptr->buffer_start = new_buffer;
    iter_ptr->buffer_end = new_buffer + (sizeof(char) * buff_size - 1);
}

int map_example_func(int data)
{
    return ++data;
}

int filter_example_func(int data)
{
    return ((data % 2) == 0); // return even numbers
}
