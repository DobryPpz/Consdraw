#include <content.h>

struct content_list *new_content_list(){
    struct content_list *l = (struct content_list*)malloc(sizeof(struct content_list));
    if(l==NULL) return NULL;
    l->head = NULL;
    return l;
}
struct content_node *new_content_node(char **content, int content_width, int content_height){
    struct content_node *n = (struct content_node*)malloc(sizeof(struct content_node));
    if(n==NULL) return NULL;
    n->content = content;
    n->content_width = content_width;
    n->content_height = content_height;
    n->next = NULL;
    return n;
}
void destroy_content_list(struct content_list *l){
    if(l==NULL) return;
    if(l->head!=NULL){
        struct content_node *traverser = NULL;
        while(l->head!=NULL){
            traverser = l->head;
            l->head = l->head->next;
            traverser->next = NULL;
            destroy_content_node(traverser);
        }
    }
    free(l);
}
void destroy_content_node(struct content_node *n){
    if(n==NULL) return;
    if(n->content!=NULL){
        destroy_content(n->content,n->content_height);
    }
    free(n);
}
void destroy_content(char **content, int content_height){
    if(content!=NULL){
        for(int i=0;i<content_height;i++){
            if(content[i]!=NULL){
                free(content[i]);
            }
        }
        free(content);
    }
}
bool add_content(struct content_list *l, struct content_node *n){
    if(l==NULL || n==NULL) return false;
    if(l->head==NULL){
        l->head = n;
        return true;
    }
    n->next = l->head;
    l->head = n;
    return true;
}