#include "list.h"
#include <stdlib.h>

list_t *list_create(void *const element)
{
    list_t *new_node = malloc(sizeof(list_t));

    if (new_node)
        new_node->element = element;

    return new_node;
}

bool_t list_add_front(void *const element, list_t *const header)
{
    list_t *new_node = NULL;

    if (element != NULL)
        new_node = list_create(element);

    if (!new_node)
        return false;

    new_node->next = header->next;
    new_node->prev = header;
    header->next->prev = new_node;
    header->next = new_node;

    return true;
}

bool_t list_add_back(void *const element, list_t *const header)
{
    list_t *new_node = NULL;

    if (element != NULL)
        new_node = list_create(element);

    if (!new_node)
        return false;

    new_node->next = header;
    new_node->prev = header->prev;
    header->prev->next = new_node;
    header->prev = new_node;

    return true;
}

int list_length(list_t *const header)
{
    list_t *node;
    int length = 0;

    for (node = header->next; node->element != NULL; node = node->next)
        length++;

    return length;
}

int int_list_sum(list_t *const header)
{
    list_t *node;
    int sum = 0;

    for (node = header->next; node->element != NULL; node = node->next)
    {
        sum += *((int *) (node->element));
    }

    return sum;
}

list_t *list_node_at(list_t *const header, unsigned int const index)
{
    int length = list_length(header);
    list_t *node;
    int i;

    if (index >= length)
        return NULL;

    if (index <= length / 2)
    {
        node = header->next;
        for (i = 0; i != index; i++)
            node = node->next;
    }
    else
    {

        node = header->prev;
        for (i = length - 1; i != index; i--)
            node = node->prev;
    }

    return node;
}

void list_free(list_t *const header) {

    list_t *node;
    node = header->next;

    while (node->element)
    {
        free(node->element);
        node = node->next;
        free(node->prev);
    }

    header->next = header;
    header->prev = header;
}


