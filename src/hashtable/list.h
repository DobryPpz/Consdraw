#ifndef LIST_H_
#define LIST_H_
#include <stdlib.h>
#include <string.h>

typedef struct ListElmt{
    void *data;
    struct ListElmt *next;
}ListElmt;

typedef struct List{
    void (*destroy)(void*);
    ListElmt *head;
    ListElmt *tail;
    int size;
}List;

void list_init(List *list, void (*destroy)(void*));
void list_destroy(List *list);
int list_ins_next(List *list, ListElmt *element, void *data);
int list_rem_next(List *list, ListElmt *element, void **data);

#endif