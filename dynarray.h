#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdlib.h>
#include <string.h>
#define START_SIZE 4



struct dynarray
{
	void ** buffer;
	unsigned int size;
	unsigned int count;
};
typedef struct dynarray dynarray;

typedef void (*dynarray_forfn)(void *);

dynarray * dynarray_create(unsigned int startsize);
void dynarray_empty(dynarray *array);
void dynarray_delete(dynarray *array);
void dynarray_add_tail(dynarray *array, void * data);
void dynarray_add_head(dynarray *array, void * data);
void * dynarray_remove_tail(dynarray * array);
void * dynarray_remove_head(dynarray * array);
void dynarray_insert(dynarray *array, unsigned int pos, void *data);
void * dynarray_remove(dynarray *array, unsigned int pos);
void * dynarray_get(const dynarray *array, unsigned int pos);
void * dynarray_set(dynarray *array, unsigned int pos, void *data);
void dynarray_for_each(const dynarray *array, dynarray_forfn fun);
unsigned int dynarray_get_count(const dynarray *array);
void dynarray_set_size(dynarray *array, unsigned int size);

// Implementation

dynarray * dynarray_create(unsigned int size)
{
	dynarray * array = malloc(sizeof(dynarray));
	if (array != NULL)
	{
		if (size)
		{
			array->buffer = malloc(size * sizeof(void*));
			if (array->buffer)
			{
				array->size = size;
			}
			else
			{
				free(array);
				array = NULL;
			}
		}
		else
		{
			array->buffer = NULL;
			array->size = 0;
		}
		array->count = 0;
	}
	return array;
}

void dynarray_empty(dynarray * array)
{
	array->count = 0;
}

void dynarray_delete(dynarray * array)
{
	if (array)
	{
		free(array->buffer);
		free(array);
	}
}

void dynarray_add_tail(dynarray * array, void *data)
{
	if (array->count == array->size)
	{
		if (array->buffer != NULL)
		{
			void **buffer = realloc(array->buffer, array->size * 2 * sizeof(void*));
			array->buffer = buffer;
			array->size *= 2;
		}
		else
		{
			array->buffer = malloc(START_SIZE * sizeof(void*));
			array->size = START_SIZE;
		}
	}
	if (array->buffer != NULL)
	{
		array->buffer[array->count] = data;
		array->count++;
	}
}

void dynarray_add_head(dynarray * array, void *data)
{
	if (array->count == array->size)
	{
		if (array->buffer != NULL)
		{
			void **temp = malloc(array->size * 2 * sizeof(void*));
			if (temp)
			{
				memcpy(temp + 1, array->buffer, array->count * sizeof(void*));
				free(array->buffer);
				array->buffer = temp;
				array->size *= 2;
			}
		}
		else
		{
			array->buffer = malloc(START_SIZE * sizeof(void*));
			if (array->buffer)
			{
				array->size = START_SIZE;
			}
		}
	}
	else
	{
		memmove(array->buffer + 1, array->buffer, array->count * sizeof(void*));
	}
	if(array->buffer != NULL)
	{
		array->buffer[0] = data;
		array->count++;
	}
}

void * dynarray_remove_tail(dynarray * array)
{
	void * data = NULL;
	if (array->count > 0)
	{
		data = array->buffer[array->count - 1];
		array->count--;
	}
	return data;
}

void * dynarray_remove_head(dynarray * array)
{
	void * data = NULL;
	if (array->count > 0)
	{
		data = array->buffer[0];
		memmove(array->buffer, array->buffer + 1, (array->count - 1) * sizeof(void*));
		array->count--;
	}
	return data;
}

void dynarray_insert(dynarray * array, unsigned int pos, void *data)
{
	if (pos == 0)
	{
		dynarray_add_head(array, data);
	}
	else if (pos == array->count)
	{
		dynarray_add_tail(array, data);
	}
	else if (pos < array->count)
	{
		unsigned int i;
		if (array->count == array->size)
		{
			void **temp = malloc(array->size * 2 * sizeof(void*));
			if (temp)
			{
				memcpy(temp, array->buffer, pos * sizeof(void*));
				memcpy(temp + pos + 1, array->buffer + pos, (array->count - pos) * sizeof(void*));
				free(array->buffer);
				array->buffer = temp;
				array->size *= 2;
			}
		}
		else
		{
			memmove(array->buffer + pos + 1, array->buffer + pos, (array->count - pos) * sizeof(void*));
		}
		array->buffer[pos] = data;
		array->count++;
	}
}

void * dynarray_remove(dynarray *array, unsigned int pos)
{
	void *data;
	if (array->count < pos + 1)
	{
		data = NULL;
	}
	else if (pos == 0)
	{
		data = dynarray_remove_head(array);
	}
	else if (pos  == array->count - 1)
	{
		data = dynarray_remove_tail(array);
	}
	else
	{
		unsigned int i;
		data = array->buffer[pos];
		memmove(array->buffer + pos, array->buffer + pos + 1, (array->count - pos - 1) * sizeof(void*));
		array->count--;
	}
	return data;
}

void * dynarray_get(const dynarray * array, unsigned int pos)
{
	void * data = NULL;
	if (pos < array->count)
	{
		data = array->buffer[pos];
	}
	return data;
}

void * dynarray_set(dynarray * array, unsigned int pos, void * data)
{
	void * temp = NULL;
	if (pos == array->count)
	{
		dynarray_add_tail(array, data);
	}
	else if (pos < array->count)
	{
		temp = array->buffer[pos];
		array->buffer[pos] = data;
	}
	return temp;
}

void dynarray_set_size(dynarray * array, unsigned int size)
{
	array->buffer = realloc(array->buffer, size);
	if (array->buffer)
	{
		array->size = size;
		if (array->size < array->count)
		{
			array->count = array->size;
		}
	}
	else
	{
		array->size = 0;
		array->count = 0;
	}
}

void dynarray_for_each(const dynarray * array, dynarray_forfn fun)
{
	unsigned int i;
	for (int i = 0; i < array->count; i++)
	{
		fun(array->buffer[i]);
	}
}

unsigned int dynarray_get_count(const dynarray * array)
{
	return array->count;
}


#endif