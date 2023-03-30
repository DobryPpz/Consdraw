#include <scene.h>
#include <utilities.h>

void destroy_scene(struct scene *s){
    if(s!=NULL){
        if(s->head!=NULL){
            struct element *traverser = NULL;
            while(s->head!=NULL){
                traverser = s->head;
                s->head = s->head->next;
                traverser->next = NULL;
                traverser->content = NULL;
                free(traverser->id);
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
        memset(s->canvas[i],' ',s->width);
    }
    return s;
}
struct element *new_element(char *id, int x, int y, int content_height, int content_width, char **content){
    struct element *el = (struct element*)malloc(sizeof(struct element));
    if(el==NULL) return NULL;
    el->id = (char*)calloc(strlen(id)+1,sizeof(char));
    strcpy(el->id,id);
    el->x = x;
    el->y = y;
    el->content_height = content_height;
    el->content_width = content_width;
    el->content = content;
    el->next = NULL;
    return el;
}
struct element *get_element(struct scene *s, char *id){
    if(s!=NULL && s->head!=NULL){
        struct element *traverser = s->head;
        while(traverser!=NULL && strcmp(traverser->id,id)!=0){
            traverser = traverser->next;
        }
        if(traverser!=NULL && strcmp(traverser->id,id)==0){
            return traverser;
        }
        else{
            return NULL;
        }
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
bool remove_from_scene(struct scene *s, char *id){
    if(s!=NULL && s->head!=NULL){
        struct element *traverser = s->head;
        if(strcmp(traverser->id,id)==0){
            s->head = s->head->next;
            traverser->next = NULL;
            free(traverser);
            return true;
        }
        while(traverser->next!=NULL && strcmp(traverser->next->id,id)!=0){
            traverser = traverser->next;
        }
        if(traverser->next==NULL) return false;
        struct element *to_remove = traverser->next;
        s->tail = traverser;
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
            for(int i=traverser->y;i>=0&&i<traverser->content_height+traverser->y&&i<s->height;i++){
                for(int j=traverser->x;j>=0&&j<traverser->content_width+traverser->x&&j<s->width;j++){
                    char c = traverser->content[i-traverser->y][j-traverser->x];
                    if(c!='\0' && c!=' '){
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
            for(int j=0;j<s->width;j++){
                printf("%c",s->canvas[i][j]);
            }
            printf("\n");
        }
    }
}
void clear_canvas(struct scene *s){
    if(s!=NULL && s->canvas!=NULL){
        for(int i=0;i<s->height;i++){
            for(int j=0;j<s->width;j++){
                s->canvas[i][j] = ' ';
            }
        }
    }
}
void clear_screen(struct scene *s){
    clear_canvas(s);
    system("clear");
    load_scene(s);
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
void down_element(struct scene *s, struct element *el, int howManyLayers){
    if(s->head == el) return;
    struct element *traverser = s->head;
    struct element *current = NULL;
    struct element *subsequent = NULL;
    int current_index = 1;
    int new_index;
    while(traverser->next!=el){
        traverser = traverser->next;
        current_index++;
    }
    traverser->next = el->next;
    el->next = NULL;
    new_index = max_int(0,current_index-howManyLayers);
    if(new_index==0){
        el->next = s->head;
        s->head = el;
        return;
    }
    current = s->head;
    subsequent = s->head->next;
    for(int i=1;i<new_index && subsequent;i++){
        subsequent = subsequent->next;
        current = current->next;
    }
    current->next = el;
    el->next = subsequent;
}