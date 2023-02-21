#include <stdbool.h>
#include <stdlib.h>
#include <palette.h>
#include <string.h>

struct palette *new_palette(){
    struct palette *p = (struct palette*)malloc(sizeof(struct palette));
    p->root = NULL;
    return p;
}
void rot_left(struct palette *p, struct drawing *axis, struct drawing *parent){
    if(p!=NULL && axis!=NULL){
        struct drawing *right_subtree = axis->right;
        if(right_subtree->left!=NULL){
            axis->right = right_subtree->left;
        }
        if(parent==NULL) p->root = right_subtree;
        else if(parent->left==axis){
            parent->left = right_subtree;
        }
        else{
            parent->right = right_subtree;
        }
        right_subtree->left = axis;
    }
}
void rot_right(struct palette *p, struct drawing *axis, struct drawing *parent){
    if(p!=NULL && axis!=NULL){
        struct drawing *left_subtree = axis->left;
        if(left_subtree->right!=NULL){
            axis->left = left_subtree->right;
        }
        if(parent==NULL) p->root = left_subtree;
        else if(parent->right==axis){
            parent->right = left_subtree;
        }
        else{
            parent->left = left_subtree;
        }
        left_subtree->right = axis;
    }
}
void left_right(struct palette *p, struct drawing *axis, struct drawing *parent){
    if(p!=NULL && axis!=NULL){
        rot_left(p,axis->left,axis);
        rot_right(p,axis,parent);
    }
}
void right_left(struct palette *p, struct drawing *axis, struct drawing *parent){
    if(p!=NULL && axis!=NULL){
        rot_right(p,axis->right,axis);
        rot_left(p,axis,parent);
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
    if(p!=NULL && d!=NULL){
        if(p->root==NULL){
            p->root = d;
            return true;
        }
        else{

        }
    }
    else{
        return false;
    }
}
struct drawing *get_drawing(struct palette *p, char *name){
    if(p!=NULL && p->root!=NULL && name!=NULL){
        struct drawing *traverser = p->root;
        while(traverser!=NULL){
            int cmp = strcmp(name,traverser->name);
            if(cmp==0) return traverser;
            else if(cmp<0) traverser=traverser->left;
            else traverser=traverser->right;
        }
        return NULL;
    }
    else{
        return NULL;
    }
}