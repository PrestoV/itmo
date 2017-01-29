#ifndef LIST_LSP_LIST_H
#define LIST_LSP_LIST_H

typedef enum { false, true } bool_t;

typedef struct list_t {
    void *element;
    struct list_t *next;
    struct list_t *prev;
} list_t;

list_t *list_create(void *const element);
bool_t list_add_front(void *const element, list_t *const header);
bool_t list_add_back(void *const element, list_t *const header);
void list_free(list_t *const header);
int list_length(list_t *const header);
list_t *list_node_at(list_t *const header, unsigned int const index);
int int_list_sum(list_t *const header);

#endif

