#ifndef _LLIST_H_
#define _LLIST_H_

#include "mem.h"

struct mem_t;

struct mem_t* mem_get_space(struct mem_t* list, size_t request);
struct mem_t* mem_get_block(struct mem_t* list, char* ptr);
struct mem_t* mem_get_prev(struct mem_t* list, struct mem_t* blk);

#endif
