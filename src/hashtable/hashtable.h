#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_
#include <list.h>
#include <stdio.h>

typedef struct hash_table{
    unsigned int buckets;
    unsigned int (*h)(const void*);
    const void* (*get_key)(void*);
    int size;
    int (*match)(const void*, const void*);
    void (*destroy)(void*);
    List *table;
}hash_table;

unsigned int hashpjw(const void *key);
int hash_table_init(hash_table *htbl, 
                    unsigned int buckets, 
                    unsigned int (*h)(const void*), 
                    int (*match)(const void*, const void*),
                    void (*destroy)(void*),
                    const void* (*get_key)(void*));
void hash_table_destroy(hash_table *htbl);
int hash_table_lookup(hash_table *htbl, const void *key, void **data);
int hash_table_insert(hash_table *htbl, const void *key, void *data);
int hash_table_remove(hash_table *htbl, const void *key, void **data);
int hash_table_upgrade(hash_table *htbl);

#endif