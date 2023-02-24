#include <programstate.h>

void draw_command(FILE *fp, char **saveptr, struct context *c){
    char name[32] = {'\0'};
    char relation[32] = {'\0'};
    char drawing[32] = {'\0'};
    int x;
    int y;
    struct drawing *d = NULL;
    struct element *el = NULL;
    if(sscanf(*saveptr,"%s %s %s %d %d",name,relation,drawing,&x,&y)<5){
        printf("not enough arguments to function call: draw\n");
    }
    else{
        el = get_element(c->scene,name);
        if(el!=NULL){
            printf("the element called like that already exists in the scene %s %s\n",name,el->id);
            return;
        }
        if(strcmp(relation,"=")!=0) return;
        d = get_drawing(c->palette,drawing);
        if(d==NULL){
            printf("the is no drawing with this name in palette memory\n");
            return;
        }
        el = new_element(name,x,y,d->content_height,d->content_width,d->content);
        if(el==NULL){
            return;
        }
        if(!add_to_scene(c->scene,el)){
            return;
        }
        clear_screen(c->scene);
        draw_scene(c->scene);
    }
}
void delete_command(FILE *fp, char **saveptr, struct context *c){
    char name[32] = {'\0'};
    struct drawing *d;
    if(sscanf(*saveptr,"%s",name)<1){
        printf("scene element to delete was not specified\n");
        return;
    }
    remove_from_scene(c->scene,name);
    clear_screen(c->scene);
    draw_scene(c->scene);
}
void menu_command(FILE *fp, char **saveptr, struct context *c){
    if(c!=NULL){
        destroy_palette(c->palette);
        c->palette = NULL;
        destroy_scene(c->scene);
        c->scene = NULL;
        change_state(c,stdin,read_menu);
    }
}
void end_command(FILE *fp, char **saveptr, struct context *c){
    if(c!=NULL){
        destroy_palette(c->palette);
        c->palette = NULL;
        destroy_scene(c->scene);
        c->scene = NULL;
        destroy_context(c);
        c = NULL;
        exit(EXIT_SUCCESS);
    }
}
void move_command(FILE *fp, char **saveptr, struct context *c){
    if(c!=NULL){
        char name[32] = {'\0'};
        struct element *el = NULL;
        int new_x;
        int new_y;
        if(sscanf(*saveptr,"%s %d %d",name,&new_x,&new_y)<3){
            printf("not enough arguments to function call: move\n");
        }
        else{
            el = get_element(c->scene,name);
            if(el!=NULL){
                move_element(el,new_x,new_y);
                clear_screen(c->scene);
                draw_scene(c->scene);
            }
            else{
                printf("the element with that name does does not exists in the scene\n");
            }
        }
    }
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
                        free(line);
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
    c->palette = new_palette();
    bool read_width = false;
    bool read_height = false;
    bool is_reading_shape = false;
    FILE *handle = NULL;
    char *token = NULL;
    size_t linelen = 128;
    char *line = (char*)calloc(linelen,sizeof(char));
    char **content = NULL;
    char *name = NULL;
    int content_height;
    int content_width;
    int scene_width;
    int scene_height;
    while(getline(&line,&linelen,fp)!=EOF){
        if(is_reading_shape){
            if(line[0]=='e' && line[1]=='n' && line[2]=='d' && is_reading_shape){
                is_reading_shape = false;
                struct drawing *d = new_drawing(name,content,content_height,content_width);
                add_drawing(c->palette,d);
                name = NULL;
                content = NULL;
                content_height = 0;
            }
            else{
                content_height++;
                line[strlen(line)-1] = '\0';
                content = (char**)realloc(content,content_height*sizeof(char*));
                content[content_height-1] = (char*)calloc(content_width,sizeof(char));
                sprintf(content[content_height-1],"%s",line);
            }
        }
        else{
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
            }
        }
        memset(line,'\0',linelen);
    }
    fclose(fp);
    free(line);
    change_state(c,stdin,read_drawing);
}
void read_drawing(FILE *fp, struct context *c){
    char *token = NULL;
    int x;
    int y;
    size_t linelen = 128;
    char *line = (char*)calloc(linelen,sizeof(char));
    char *saveptr = NULL;
    while(getline(&line,&linelen,fp)!=EOF){
        if(strlen(line)>1){
            token = strtok_r(line," \n\t",&saveptr);
            if(strcmp(token,"draw")==0) draw_command(fp,&saveptr,c);
            else if(strcmp(token,"delete")==0) delete_command(fp,&saveptr,c);
            else if(strcmp(token,"move")==0) move_command(fp,&saveptr,c);
            else if(strcmp(token,"menu")==0) {
                menu_command(fp,&saveptr,c);
                break;
            }
            else if(strcmp(token,"end")==0) {
                end_command(fp,&saveptr,c);
                break;
            }
            saveptr = NULL;
            memset(line,'\0',linelen);
        }
    }
    free(line);
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