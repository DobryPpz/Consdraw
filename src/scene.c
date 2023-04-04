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
struct element *shift(struct scene *s){
    if(!s || !s->head) return NULL;
    struct element *ret = s->head;
    s->head = s->head->next;
    ret->next = NULL;
    return ret;
}
struct stack *new_stack(){
    struct stack *s = (struct stack*)malloc(sizeof(struct stack));
    s->top = NULL;
    return s;
}
bool destroy_stack(struct stack *s){
    if(!s) return false;
    struct element *to_destroy = NULL;
    while(s->top){
        to_destroy = pop(s);
        to_destroy->next = NULL;
        free(to_destroy);
    }
    free(s);
    return true;
}
bool push(struct stack *s, struct element *el){
    if(!s || !el) return false;
    if(!s->top){
        s->top = el;
        return true;
    }
    el->next = s->top;
    s->top = el;
    return true;
}
struct element *pop(struct stack *s){
    if(!s || !s->top) return NULL;
    struct element *to_pop = s->top;
    s->top = s->top->next;
    to_pop->next = NULL;
    return to_pop;
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
            free(traverser->id);
            free(traverser);
            return true;
        }
        while(traverser->next!=NULL && strcmp(traverser->next->id,id)!=0){
            traverser = traverser->next;
        }
        if(traverser->next==NULL) return false;
        struct element *to_remove = traverser->next;
        if(to_remove->next==NULL){
            s->tail = traverser;
        }
        traverser->next = traverser->next->next;
        to_remove->next = NULL;
        free(to_remove->id);
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
    if(s->head->next==NULL) return;
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
    reset_tail(s);
}
void up_element(struct scene *s, struct element *el, int howManyLayers){
    if(s->tail==el) return;
    struct element *traverser = s->head;
    struct element *current = NULL;
    struct element *subsequent = NULL;
    int current_index = 0;
    int new_index;
    if(s->head->next==NULL) return;
    if(el==s->head){
        s->head = s->head->next;
        el->next = NULL;
        new_index = howManyLayers;
        current = s->head;
        subsequent = s->head->next;
        for(int i=1;i<new_index && subsequent;i++){
            subsequent = subsequent->next;
            current = current->next;
        }
        current->next = el;
        el->next = subsequent;
    }
    else{
        current_index++;
        while(traverser->next!=el){
            traverser = traverser->next;
            current_index++;
        }
        traverser->next = traverser->next->next;
        el->next = NULL;
        new_index = current_index+howManyLayers;
        current = s->head;
        subsequent = s->head->next;
        for(int i=1;i<new_index && subsequent;i++){
            subsequent = subsequent->next;
            current = current->next;
        }
        current->next = el;
        el->next = subsequent;
    }
    reset_tail(s);
}
void bottom_element(struct scene *s, struct element *el){
    if(s->head==el || s->head->next==NULL) return;
    struct element *traverser = s->head;
    while(traverser->next!=el){
        traverser = traverser->next;
    }
    traverser->next = el->next;
    el->next = s->head;
    s->head = el;
    reset_tail(s);
}
void top_element(struct scene *s, struct element *el){
    if(s->tail==el || s->head->next==NULL) return;
    struct element *traverser = s->head;
    if(s->head==el){
        s->head = s->head->next;
        el->next = NULL;
        s->tail->next = el;
        s->tail = el;
    }
    else{
        while(traverser->next!=el){
            traverser = traverser->next;
        }
        traverser->next = el->next;
        el->next = NULL;
        s->tail->next = el;
        s->tail = el;
    }
}
void list_elements(struct scene *s){
    if(!s) return;
    clear_screen(s);
    struct element *traverser = s->head;
    while(traverser){
        printf("- %s\n",traverser->id);
        traverser = traverser->next;
    }
    printf("press any key...\n");
    getchar();
}
void show_element(struct scene *s, struct element *el){
    if(!s) return;
    clear_screen(s);
    for(int i=0;i<el->content_height;i++){
        for(int j=0;j<el->content_width;j++){
            printf("%c",el->content[i][j]);
        }
        printf("\n");
    }
    printf("x: %d   y: %d\n",el->x,el->y);
    printf("press any key...\n");
    getchar();
}
void reverse_elements(struct scene *s){
    if(!s) return;
    struct stack *reverser = new_stack();
    while(s->head){
        push(reverser,shift(s));
    }
    while(reverser->top){
        add_to_scene(s,pop(reverser));
    }
    destroy_stack(reverser);
}
void reset_tail(struct scene *s){
    if(!s) return;
    struct element *traverser = s->head;
    while(traverser->next!=NULL){
        traverser = traverser->next;
    }
    s->tail = traverser;
}