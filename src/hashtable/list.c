#include <list.h>

void list_init(List *list, void (*destroy)(void*)){
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}
void list_destroy(List *list){
    void *data = NULL;
    while(list_rem_next(list,NULL,(void**)&data)==0){
        if(list->destroy!=NULL){
            list->destroy(data);
        }
    }
    memset(list,0,sizeof(List));
}
int list_ins_next(List *list, ListElmt *element, void *data){
    ListElmt *new_element = NULL;
    if((new_element=(ListElmt*)malloc(sizeof(ListElmt)))==NULL){
        return -1;
    }
    new_element->data = (void*)data;
    if(element==NULL){
        new_element->next = list->head;
        list->head = new_element;
        if(list->tail==NULL){
            list->tail = new_element;
        }
    }
    else{
        new_element->next = element->next;
        element->next = new_element;
        if(new_element->next==NULL){
            list->tail = new_element;
        }
    }
    list->size++;
    return 0;
}
int list_rem_next(List *list, ListElmt *element, void **data){
    if(list->size==0){
        return -1;
    }
    ListElmt *old_element = NULL;
    if(element==NULL){
        old_element = list->head;
        list->head = list->head->next;
        if(list->head==NULL){
            list->tail = NULL;
        }
    }
    else{
        if(element->next==NULL){
            return -1;
        }
        old_element = element->next;
        element->next = element->next->next;
        if(element->next==NULL){
            list->tail = element;
        }
    }
    *data = old_element->data;
    free(old_element);
    list->size--;
    return 0;
}