#include "list.h"

#ifndef LAB5_HIGH_FUNC_H
#define LAB5_HIGH_FUNC_H

void foreach(list_t* const header, void (*const func)(void *));
list_t* map(list_t* const header, void* (*const func)(void *));
void map_mut(list_t* const header, void* (*const func)(void *));
void* foldl(void* accum, void* (*const func)(void*, void*), list_t* const header);
list_t* iterate(void* const s, int const n, void* (*const func)(void *));
#endif
