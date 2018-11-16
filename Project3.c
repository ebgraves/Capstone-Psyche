#include <stdio.h>
#include "minheap.h"

int main(void)
{
	minheap *heap = minheap_create();
	char characters[10];
	for (int i = 0; i < 10; i++)
	{
		characters[i] = 97 + i;
		minheap_add(heap, &(characters[i]), i);
	}

	for (int i = 0; i < 10; i++)
	{
		printf("characters[%d] = %p\n", i, *(heap->entries->count));
	}
	return 0;

}