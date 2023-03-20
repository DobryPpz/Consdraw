#include <stdbool.h>
#include <stdlib.h>
#include <palette.h>
#include <string.h>

struct palette *new_palette(){
    struct palette *p = (struct palette*)malloc(sizeof(struct palette));
    p->root = NULL;
    return p;
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
    if(d->name!=NULL){
        free(d->name);
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
            bool inserted = false;
            struct drawing *traverser = p->root;
            while(!inserted){
                int cmp = strcmp(d->name,traverser->name);
                if(cmp<0){
                    if(traverser->left!=NULL){
                        traverser = traverser->left;
                    }
                    else{
                        traverser->left = d;
                        inserted = true;
                    }
                }
                else{
                    if(traverser->right!=NULL){
                        traverser = traverser->right;
                    }
                    else{
                        traverser->right = d;
                        inserted = true;
                    }
                }
            }
            return true;
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