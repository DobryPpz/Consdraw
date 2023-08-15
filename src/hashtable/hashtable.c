#include <hashtable.h>

unsigned int hashpjw(const void *key){
    const char *ptr;
    unsigned int val;
    val = 0;
    ptr = key;
    while(*ptr!='\0'){
        unsigned int tmp;
        val = (val<<4)+(*ptr);
        if((tmp=(val&0xf0000000))){
            val = val^(tmp>>24);
            val = val^tmp;
        }
        ptr++;
    }
    return val;
}
int hash_table_init(hash_table *htbl, 
                    unsigned int buckets, 
                    unsigned int (*h)(const void*), 
                    int (*match)(const void*, const void*),
                    void (*destroy)(void*),
                    const void* (*get_key)(void*)){
    if((htbl->table=(List*)calloc(buckets,sizeof(List)))==NULL){
        return -1;
    }
    for(unsigned int i=0;i<buckets;i++){
        list_init(&htbl->table[i],destroy);
    }
    htbl->buckets = buckets;
    htbl->h = h;
    htbl->size = 0;
    htbl->match = match;
    htbl->destroy = destroy;
    htbl->get_key = get_key;
    return 0;
}
void hash_table_destroy(hash_table *htbl){
    for(unsigned int i=0;i<htbl->buckets;i++){
        list_destroy(&htbl->table[i]);
    }
    free(htbl->table);
    memset(htbl,0,sizeof(hash_table));
}
int hash_table_lookup(hash_table *htbl, const void *key, void **data){
    ListElmt *member = NULL;
    unsigned int hashval = (unsigned int)htbl->h(key)%htbl->buckets;
    List *list = &htbl->table[hashval];
    for(member=list->head;member!=NULL;member=member->next){
        if(htbl->match(htbl->get_key(member->data),key)){
            *data = (void*)member->data;
            return 0;
        }
    }
    return -1;
}
int hash_table_insert(hash_table *htbl, const void *key, void *data){
    void *tmp = NULL;
    if(hash_table_lookup(htbl,key,(void**)&tmp)==0){
        return -1;
    }
    unsigned int hashval = (unsigned int)htbl->h(key)%htbl->buckets;
    List *list = &htbl->table[hashval];
    if(list_ins_next(list,list->tail,data)!=0){
        return -1;
    }
    htbl->size++;
    if(2*htbl->buckets<htbl->size){
        hash_table_upgrade(htbl);
    }
    return 0;
}
int hash_table_remove(hash_table *htbl, const void *key, void **data){
    ListElmt *member = NULL;
    ListElmt *prev = NULL;
    unsigned int hashval = (unsigned int)htbl->h(key)%htbl->buckets;
    List *list = &htbl->table[hashval];
    for(member=list->head;member!=NULL;member=member->next){
        if(htbl->match(htbl->get_key(member->data),key)){
            printf("we found the one to remove in hash table\n");
            getchar();
            if(list_rem_next(list,prev,(void**)&data)==0){
                htbl->size--;
                return 0;
            }
            else{
                return -1;
            }
        }
        prev = member;
    }
    return -1;
}
int hash_table_upgrade(hash_table *htbl){
    List *old_table = htbl->table;
    List *list = NULL;
    void *data = NULL;
    unsigned int old_buckets = htbl->buckets;
    htbl->table = NULL;
    if((htbl->table=(List*)calloc(2*htbl->buckets,sizeof(List)))==NULL){
        return -1;
    }
    htbl->buckets *= 2;
    for(unsigned int i=0;i<htbl->buckets;i++){
        list_init(&htbl->table[i],htbl->destroy);
    }
    for(unsigned int i=0;i<old_buckets;i++){
        list = &old_table[i];
        while(list_rem_next(list,NULL,(void**)&data)==0){
            if(hash_table_insert(htbl,htbl->get_key(data),data)!=0){
                return -1;
            }
        }
    }
    free(old_table);
    return 0;
}