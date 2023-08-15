#include <palette.h>

const void *get_drawing_key(void *drawing){
    struct drawing *drw = (struct drawing*)drawing;
    return (const void*)drw->name;
}
int drawing_cmp(const void *drw1, const void *drw2){
    char *n1 = (char*)drw1;
    char *n2 = (char*)drw2;
    if(strcmp(n1,n2)==0){
        return 1;
    }
    return 0;
}
void destroy_drawing(void *drawing){
    struct drawing *drw = (struct drawing*)drawing;
    if(drw->name){
        free(drw->name);
    }
    free(drw);
}
struct palette *new_palette(){
    struct palette *p = (struct palette*)malloc(sizeof(struct palette));
    if(hash_table_init(&p->table,2,hashpjw,drawing_cmp,destroy_drawing,get_drawing_key)!=0){
        free(p);
        return NULL;
    }
    return p;
}
void destroy_palette(struct palette *p){
    if(!p) return;
    hash_table_destroy(&p->table);
    free(p);
}
struct drawing *new_drawing(char *name, char **content, int content_height, int content_width){
    struct drawing *d = (struct drawing*)malloc(sizeof(struct drawing));
    d->name = name;
    d->content = content;
    d->content_height = content_height;
    d->content_width = content_width;
    d->height = 0;
    return d;
}
bool add_drawing(struct palette *p, struct drawing *d){
    if(!(p && d)) return false;
    if(hash_table_insert(&p->table,p->table.get_key(d),d)!=0){
        return false;
    }
    return true;
}
struct drawing *get_drawing(struct palette *p, char *name){
    if(!(p && name)) return NULL;
    struct drawing *drawing = NULL;
    if(hash_table_lookup(&p->table,name,(void**)&drawing)!=0){
        return NULL;
    }
    return drawing;
}