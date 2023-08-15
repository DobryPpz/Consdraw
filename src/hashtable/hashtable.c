#include <hashtable.h>

unsigned int hashpjw(const void *key);
int hash_table_init(hash_table *htbl, 
                    unsigned int buckets, 
                    unsigned int (*h)(const void*), 
                    int (*match)(const void*, const void*),
                    void (*destroy)(void*)){
    
}
void hash_table_destroy(hash_table *htbl){

}
int hash_table_lookup(hash_table *htbl, const void *key, void **data){

}
int hash_table_insert(hash_table *htbl, const void *key, void *data){

}
int hash_table_remove(hash_table *htbl, const void *key, void **data){

}