#include "high_func.h"
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

void foreach(list_t *const header, void (*const func)(void *)) {
    list_t* node;

    for(node = header->next; node->element != NULL; node = node->next)
        func(node->element);
}

list_t *map(list_t *const header, void *(*const func)(void *)) {
    list_t *new_header;
    list_t *node;
    void* res;

    new_header = list_create(NULL);
    if(!new_header)
        return NULL;

    new_header->next = new_header;
    new_header->prev = new_header;

    for(node = header->next; node->element != NULL; node = node->next)
    {
        res = func(node->element);

            list_add_back(res, new_header);
    }

    return new_header;
}

void map_mut(list_t *const header, void *(*const func)(void *)) {
    list_t* node;
    void* res;

    for(node = header->next; node->element != NULL; node = node->next)
    {
        res = func(node->element);
        if(res)
            node->element = res;
    }

}

void *foldl(void *accum, void *(*const func)(void *, void *), list_t *const header) {
    list_t *node;

    for(node = header->next; node->element != NULL; node = node->next)
    {
        accum = func(accum, node->element);
    }

    return accum;
}

list_t *iterate(void *const s, int const n, void *(*const func)(void *)) {
    list_t* new_header;
    void* res;
    int i;

    res = s;
    new_header = list_create(NULL);
    if(!new_header)
        return NULL;

    new_header->next = new_header;
    new_header->prev = new_header;

    for(i = 0; i < n; i++)
    {
        if(res)
            list_add_back(res, new_header);

        res = func(res);
    }

    return new_header;
}
