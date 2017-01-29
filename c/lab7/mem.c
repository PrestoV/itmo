#include <stdio.h>
#include <unistd.h>
#include "mem.h"
#include "mem_list.h"

static struct mem_t *head_heap = NULL;

static size_t get_size(const size_t query)
{
    long page_size = sysconf(_SC_PAGESIZE);
	return query % page_size ? (query/page_size + 1) * page_size : query;
}

void* heap_init( size_t initial_size )
{
	head_heap = mmap(HEAP_START, initial_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);

    if(head_heap == MAP_FAILED)
    {
        head_heap = mmap(NULL, initial_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if(head_heap == MAP_FAILED)
            head_heap = NULL;
    }

    if(head_heap != NULL)
    {
        head_heap->next = NULL;
        head_heap->capacity = get_size(initial_size) - sizeof(struct mem_t);
        head_heap->is_free = 1;
    }

    return head_heap;
}

static struct mem_t* extend_block(struct mem_t *ptr, size_t query)
{
    struct mem_t *new = mmap( (char*)ptr + sizeof(struct mem_t) + ptr->capacity,
                      get_size(query), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);

    if (new == MAP_FAILED)
        return NULL;

    ptr->capacity += query;
    return ptr;
}

static struct mem_t* allocate_new_block(struct mem_t *ptr, size_t query)
{
    struct mem_t *new = mmap(NULL, get_size(query + sizeof(struct mem_t)), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (ptr == MAP_FAILED)
        return NULL;

    ptr->next = new;

    new->capacity = get_size(query + sizeof(struct mem_t)) - sizeof(struct mem_t);
    new->is_free = 1;
    new->next = NULL;

    return new;
}

static struct mem_t* divide(struct mem_t* ptr, size_t query)
{
	struct mem_t* new = NULL;

	if(ptr != NULL && ptr->capacity >= sizeof(struct mem_t) + query)
    {
		new = (struct mem_t*)((char*)ptr + sizeof(struct mem_t) + query);
		new->capacity = ptr->capacity - sizeof(struct mem_t) - query;
		new->is_free = 1;
		new->next = ptr->next;
		
		ptr->capacity = query;
		ptr->next = new;
	}
	return ptr;
}

void* _malloc( size_t query )
{
	struct mem_t* ptr = mem_get_space(head_heap, query);

	if(ptr != NULL)
    {
        ptr = divide(ptr, query);
	}
    else
    {
		ptr = mem_get_prev(head_heap, NULL);

        if(ptr->is_free)
        {
            if(extend_block(ptr, query - ptr->capacity) == NULL)
            {
                ptr = divide(allocate_new_block(ptr, query), query);
            }

        }
        else
        {
            ptr = divide(allocate_new_block(ptr, query), query);
        }
	}

    ptr->is_free = 0;

	return (char*)ptr + sizeof(struct mem_t);
}


void _free( void* mem )
{
	struct mem_t* cur_block = mem_get_block(head_heap, mem);
	struct mem_t* prev_block = mem_get_prev(head_heap, cur_block);
	
    if(cur_block != NULL)
    {
		cur_block->is_free = 1;

		if(cur_block->next != NULL)
        {
			if( (cur_block->next->is_free == 1)  &&
                    ((char*)cur_block->next == (char*)cur_block + sizeof(struct mem_t) + cur_block->capacity) )
            {
				cur_block->capacity += cur_block->next->capacity + sizeof(struct mem_t);
				cur_block->next = cur_block->next->next;
			}
		}

		if(prev_block != NULL)
        {
            if( (prev_block->is_free == 1)  &&
                ((char*)prev_block == (char*)cur_block - sizeof(struct mem_t) - prev_block->capacity) )
            {
				prev_block->capacity += cur_block->capacity + sizeof(struct mem_t);
				prev_block->next = cur_block->next;
			}
		}
	}
}


void* get_addr()
{
    return head_heap;
}
