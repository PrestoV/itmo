#include <stdio.h>
#include "mem.h"

int main(int argc, char **argv)
{
    void *space1, *space2, *space3;

    heap_init(1);
    printf("After init heap:\n");
    memalloc_debug_heap(stdout, get_addr());

    space1 = _malloc(1024);
    printf("\nMalloc 1024 B:\n");
    memalloc_debug_heap(stdout, get_addr());
    
    space2 = _malloc(3032);
    printf("\nMalloc 3032 B:\n");
    memalloc_debug_heap(stdout, get_addr());

    space3 = _malloc(2048);
    printf("\nMalloc 2048 B:\n");
    memalloc_debug_heap(stdout, get_addr());

    _free(space1);
    printf("\nFree 1024 B:\n");
    memalloc_debug_heap(stdout, get_addr());

    _free(space2);
    printf("\nFree 3032 B:\n");
    memalloc_debug_heap(stdout, get_addr());

    _free(space3);
    printf("\nFree 2048 B:\n");
    memalloc_debug_heap(stdout, get_addr());
    
    return 0;
}
