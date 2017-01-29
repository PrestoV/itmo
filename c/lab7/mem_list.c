#include <stdio.h>
#include "mem_list.h"

struct mem_t* mem_get_space(struct mem_t* list, size_t request)
{
	while(list != NULL)
	{
		if(list->capacity >= request && list->is_free)
			break;

		list = list->next;
	}

	return list;
}

struct mem_t* mem_get_block(struct mem_t* list, char* ptr) {
	while(list != NULL)
	{
		if(list == (struct mem_t*)(ptr - sizeof(struct mem_t)))
		{
			return list;
		}
		list = list->next;
	}
	return NULL;
}


struct mem_t* mem_get_prev(struct mem_t* list, struct mem_t* blk)
{
	while(list != NULL)
	{
		if(list->next == blk)
			break;

		list = list->next;
	}

	return list;
}

