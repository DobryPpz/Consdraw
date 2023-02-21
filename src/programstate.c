#include <programstate.h>
#include <stdlib.h>
#include <string.h>

void read_menu(FILE *fp, struct context *c){
    size_t linelen = 64;
    char *line = (char*)calloc(linelen,sizeof(char));
    char *token = NULL;
    FILE *handle = NULL;
    while(getline(&line,&linelen,fp)!=EOF){
        token = strtok(line," \n\t");
        if(token!=NULL){
            if(strcmp(token,"load")==0){
                token = strtok(NULL," \n\t");
                if(token!=NULL){
                    handle = fopen(token,"r");
                    if(handle!=NULL){
                        fclose(fp);
                        change_state(c,handle,read_parsing);
                    }
                    else{
                        printf("Could not open the file\n");
                        read_menu(fp,c);
                    }
                }
                else{
                    printf("Did not specify a file\n");
                    read_menu(fp,c);
                }
            }
            else{
                printf("Wrong command\n");
                read_menu(fp,c);
            }
        }
    }
}
void read_parsing(FILE *fp, struct context *c){

}
void read_drawing(FILE *fp, struct context *c){

}
struct context *new_context(){
    struct context *c = (struct context*)malloc(sizeof(struct context));
    c->palette = NULL;
    c->read = NULL;
    c->scene = NULL;
    return c;
}
void destroy_context(struct context *c){
    if(c!=NULL){
        if(c->palette!=NULL) destroy_palette(c->palette);
        if(c->scene!=NULL) destroy_scene(c->scene);
        free(c);
    }
}
bool change_state(struct context *c, FILE *fp, void(*read)(FILE*,struct context*)){
    if(c!=NULL && read!=NULL){
        c->read = read;
        c->read(fp,c);
    }
    else{
        return false;
    }
}