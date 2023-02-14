#include <stdbool.h>
#include <stdlib.h>
#include <palette.h>
#include <string.h>

struct palette *new_palette(){
    
}
void rot_left(struct palette *p){
    if(p!=NULL && p->root!=NULL && p->root->right!=NULL){
        struct drawing *right_subtree = p->root->right;
        p->root->right = right_subtree->left;
        right_subtree->left = p->root;
        p->root = right_subtree;
    }
}
void rot_right(struct palette *p){
    if(p!=NULL && p->root!=NULL && p->root->left!=NULL){
        struct drawing *left_subtree = p->root->left;
        p->root->left = left_subtree->right;
        left_subtree->right = p->root;
        p->root = left_subtree;
    }
}
void destroy_tree(struct drawing *d){
    if(d==NULL) return;
    destroy_tree(d->left);
    destroy_tree(d->right);
    if(d->content!=NULL){
        for(int i=0;i<d->content_height;i++){
            if(d->content[i]!=NULL) free(d->content[i]);
        }
        free(d->content);
    }
    free(d);
}
void destroy_palette(struct palette *p){
    if(p!=NULL){
        if(p->root!=NULL){
            destroy_tree(p->root);
        }
        free(p);
    }
}
struct drawing *new_drawing(char *name, char **content, int content_height, int content_width){
    struct drawing *d = (struct drawing*)malloc(sizeof(struct drawing));
    d->name = name;
    d->content = content;
    d->content_height = content_height;
    d->content_width = content_width;
    d->left = NULL;
    d->right = NULL;
    d->height = 0;
    return d;
}
bool add_drawing(struct palette *p, struct drawing *d){
    
}
struct drawing *get_drawing(struct palette *p, char *name){

}