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
                    }
                }
                else{
                    printf("Did not specify a file\n");
                }
            }
            else{
                printf("Wrong command\n");
            }
        }
    }
}
void read_parsing(FILE *fp, struct context *c){

}
void read_drawing(FILE *fp, struct context *c){

}
struct context *new_context(){

}
void destroy_context(struct context *c){

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