#include <programstate.h>

void draw(FILE *fp, char **saveptr, struct context *c){
    char name[32] = {'\0'};
    char relation[32] = {'\0'};
    char drawing[32] = {'\0'};
    int x;
    int y;
    struct drawing *d;
    struct element *el;
    if(sscanf(*saveptr,"%s %s %s %d %d",name,relation,drawing,&x,&y)<5){
        printf("not enough arguments to function call: draw\n");
    }
    else{
        el = get_element(c->scene,name);
        if(el!=NULL){
            printf("the element called like that already exists in the scene\n");
            return;
        }
        if(strcmp(relation,"=")!=0) return;
        d = get_drawing(c->palette,drawing);
        if(d==NULL){
            printf("the is no drawing with this name in palette memory\n");
            return;
        }
        el = new_element(name,x,y,d->content_height,d->content_width,d->content);
        add_to_scene(c->scene,el);
        clear_screen(c->scene);
        draw_scene(c->scene);
    }
}
void delete(FILE *fp, char **saveptr, struct context *c){
    char name[32] = {'\0'};

}
void menu(FILE *fp, char **saveptr, struct context *c){

}
void end(FILE *fp, char **saveptr, struct context *c){

}
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
    c->palette = new_palette();
    bool read_width = false;
    bool read_height = false;
    bool is_reading_shape = false;
    FILE *handle = NULL;
    char *token = NULL;
    size_t linelen = 128;
    char *line = (char*)calloc(linelen,sizeof(char));
    char **content;
    char *name;
    int content_height;
    int content_width;
    int scene_width;
    int scene_height;
    while(getline(&line,&linelen,fp)!=EOF){
        token = strtok(line," \n\t");
        if(token!=NULL){
            if(strcmp(token,"begin")==0){
                if(!(read_width && read_height)){
                    printf("file error: did not specify width and height parameters\n");
                    exit(EXIT_FAILURE);
                }
                else{
                    if(c->scene==NULL){
                        c->scene = new_scene(scene_width,scene_height);
                    }
                    token = strtok(NULL," \n\t");
                    if(token!=NULL){
                        name = (char*)calloc(scene_width,sizeof(char));
                        sscanf(token,"%s",name);
                        is_reading_shape = true;
                        content_height = 0;
                    }
                    else{
                        printf("file error: did not specify a name for the drawing\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            else if(strcmp(token,"width")==0){
                token = strtok(NULL," \n\t");
                if(token!=NULL){
                    if(sscanf(token,"%d",&scene_width)==0){
                        printf("file error: did not specify correct width value\n");
                        exit(EXIT_FAILURE);
                    }
                    scene_width = scene_width>128?128:scene_width;
                    content_width = scene_width;
                    read_width = true;
                }
                else{
                    printf("file error: did not specify any width value\n");
                    exit(EXIT_FAILURE);
                }
            }
            else if(strcmp(token,"height")==0){
                token = strtok(NULL," \n\t");
                if(token!=NULL){
                    if(sscanf(token,"%d",&scene_height)==0){
                        printf("file error: did not specify correct height value\n");
                        exit(EXIT_FAILURE);
                    }
                    read_height = true;
                }
                else{
                    printf("file error: did not specify any height value\n");
                    exit(EXIT_FAILURE);
                }
            }
            else if(strcmp(token,"end")==0 && is_reading_shape){
                is_reading_shape = false;
                struct drawing *d = new_drawing(name,content,content_height,content_width);
                add_drawing(c->palette,d);
                name = NULL;
                content = NULL;
                content_height = 0;
            }
            else if(is_reading_shape){
                content_height++;
                content = (char**)realloc(content,content_height);
                content[content_height-1] = (char*)calloc(content_width,sizeof(char));
                sprintf(content[content_height-1],"%s",token);
            }
        }
        memset(line,'\0',linelen);
    }
    change_state(c,stdin,read_drawing);
}
void read_drawing(FILE *fp, struct context *c){
    //draw t1 = triangle 0 0
    //draw sex = square 7 7
    char *token = NULL;
    int x;
    int y;
    size_t linelen = 128;
    char *line = (char*)calloc(linelen,sizeof(char));
    char **saveptr;
    while(getline(&line,&linelen,fp)!=EOF){
        token = strtok_r(line," \n\t",saveptr);
        if(strcmp(token,"draw")==0) draw(c,saveptr,c);
        else if(strcmp(token,"delete")==0) delete(c,saveptr,c);
        else if(strcmp(token,"menu")==0) menu(c,saveptr,c);
        else if(strcmp(token,"end")==0) end(c,saveptr,c);
        memset(line,'\0',linelen);
    }
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