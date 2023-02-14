#include <scene.h>
#include <stdio.h>
#include <stdlib.h>

void destroy_scene(struct scene *s){
    if(s!=NULL){
        if(s->head!=NULL){
            struct element *traverser = NULL;
            while(s->head!=NULL){
                traverser = s->head;
                s->head = s->head->next;
                free(traverser);
            }
        }
        if(s->canvas!=NULL){
            for(int i=0;i<s->height;i++){
                if(s->canvas[i]!=NULL) free(s->canvas[i]);
            }
            free(s->canvas);
        }
        free(s);
    }
}
struct scene *new_scene(int width, int height){
    struct scene *s = (struct scene*)malloc(sizeof(struct scene));
    if(s==NULL) return NULL;
    s->width = width;
    s->height = height;
    s->head = NULL;
    s->tail = NULL;
    s->canvas = (char**)calloc(s->height,sizeof(char*));
    if(s->canvas==NULL){
        destroy_scene(s);
        return NULL;
    }
    for(int i=0;i<s->height;i++){
        s->canvas[i] = (char*)calloc(s->width,sizeof(char));
        if(s->canvas[i]==NULL){
            destroy_scene(s);
            return NULL;
        }
    }
    return s;
}
struct element *new_element(int id, int x, int y, int content_height, int content_width, char **content){
    struct element *el = (struct element*)malloc(sizeof(struct element));
    if(el==NULL) return NULL;
    el->id = id;
    el->x = x;
    el->y = y;
    el->content_height = content_height;
    el->content_width = content_width;
    el->content = content;
    el->next = NULL;
    return el;
}
struct element *get_element(struct scene *s, int id){
    if(s!=NULL && s->head!=NULL){
        struct element *traverser = s->head;
        while(traverser!=NULL && traverser->id!=id){
            traverser = traverser->next;
        }
        return traverser;
    }
    else{
        return NULL;
    }
}
bool add_to_scene(struct scene *s, struct element *el){
    if(s!=NULL && el!=NULL){
        if(s->head==NULL){
            s->head = el;
            s->tail = el;
            return true;
        }
        else{
            s->tail->next = el;
            s->tail = el;
            return true;
        }
    }
    else{
        return false;
    }
}
bool remove_from_scene(struct scene *s, int id){
    if(s!=NULL && s->head!=NULL){
        struct element *traverser = s->head;
        if(traverser->id==id){
            s->head = s->head->next;
            free(traverser);
            return true;
        }
        while(traverser->next!=NULL && traverser->next->id!=id){
            traverser = traverser->next;
        }
        if(traverser->next==NULL) return false;
        struct element *to_remove = traverser->next;
        traverser->next = traverser->next->next;
        free(to_remove);
        return true;
    }
    else{
        return false;
    }
}
void load_scene(struct scene *s){
    if(s!=NULL && s->head!=NULL && s->canvas!=NULL){
        struct element *traverser = s->head;
        while(traverser!=NULL){
            for(int i=traverser->y;i>=0&&i<traverser->content_height&&i<s->height;i++){
                for(int j=traverser->x;j>=0&&j<traverser->content_width&&j<s->width;j++){
                    char c = traverser->content[i-traverser->y][j-traverser->x];
                    if(c!='\0' && c!=' ' && c!='\t' && c!='\n'){
                        s->canvas[i][j] = c;
                    }
                }
            }
            traverser = traverser->next;
        }
    }
}
void draw_scene(struct scene *s){
    if(s!=NULL && s->canvas!=NULL){
        for(int i=0;i<s->height;i++){
            printf("%s\n",s->canvas[i]);
        }
    }
}
void clear_canvas(struct scene *s){
    if(s!=NULL && s->canvas!=NULL){
        for(int i=0;i<s->height;i++){
            for(int j=0;j<s->width;i++){
                s->canvas[i][j] = '\0';
            }
        }
    }
}
void clear_screen(struct scene *s){
    clear_canvas(s);
    draw_scene(s);
}
void clear_scene(struct scene *s){
    if(s!=NULL && s->head!=NULL){
        struct element *traverser = NULL;
        while(s->head!=NULL){
            traverser = s->head;
            s->head = s->head->next;
            free(traverser);
        }
    }
    clear_screen(s);
}
void move_element(struct element *el, int x, int y){
    if(el!=NULL){
        el->x = x;
        el->y = y;
    }
}