#include <programstate.h>
#include <png.h>
#include <topng.h>

void load_command(FILE *fp, char **saveptr, struct context *c){
    char *token = NULL;
    FILE *handle = NULL;
    token = strtok_r(NULL," \n\t",saveptr);
    if(!token){
        printf("Did not specify a file\n");
        return;
    }
    handle = fopen(token,"r");
    if(handle){
        change_state(c,handle,read_parsing);
    }
    else{
        printf("Could not open the file\n");
    }
}
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
        return;
    }
    el = get_element(c->scene,name);
    if(el){
        printf("the element called like that already exists in the scene %s %s\n",name,el->id);
        return;
    }
    if(strcmp(relation,"=")) return;
    d = get_drawing(c->palette,drawing);
    if(!d){
        printf("the is no drawing with this name in palette memory\n");
        return;
    }
    el = new_element(name,x,y,d->content_height,d->content_width,d->content);
    if(!el) return;
    if(!add_to_scene(c->scene,el)) return;
    clear_screen(c->scene);
    draw_scene(c->scene);
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
    if(!c) return;
    destroy_scene(c->scene);
    c->scene = NULL;
    destroy_palette(c->palette);
    c->palette = NULL;
    change_state(c,stdin,read_menu);
}
void end_command(FILE *fp, char **saveptr, struct context *c){
    if(!c) return;
    destroy_context(c);
    c = NULL;
    exit(EXIT_SUCCESS);
}
void move_command(FILE *fp, char **saveptr, struct context *c){
    if(!c) return;
    char name[32] = {'\0'};
    struct element *el = NULL;
    int new_x;
    int new_y;
    if(sscanf(*saveptr,"%s %d %d",name,&new_x,&new_y)<3){
        printf("not enough arguments to function call: move\n");
        return;
    }
    el = get_element(c->scene,name);
    if(el){
        move_element(el,new_x,new_y);
        clear_screen(c->scene);
        draw_scene(c->scene);
        return;
    }
    printf("the element with that name does does not exists in the scene\n");
}
void down_command(FILE *fp, char **saveptr, struct context *c){
    if(!c) return;
    char name[32] = {'\0'};
    struct element *el = NULL;
    int howManyLayers;
    if(sscanf(*saveptr,"%s %d",name,&howManyLayers)<2){
        printf("not enough arguments to function call: down\n");
        return;
    }
    if(howManyLayers<=0){
        printf("the layer shift must be greater than 0\n");
        return;
    }
    el = get_element(c->scene,name);
    if(el){
        down_element(c->scene,el,howManyLayers);
        clear_screen(c->scene);
        draw_scene(c->scene);
        return;
    }
    printf("the element with that name does does not exists in the scene\n");
}
void up_command(FILE *fp, char **saveptr, struct context *c){
    if(!c) return;
    char name[32] = {'\0'};
    struct element *el = NULL;
    int howManyLayers;
    if(sscanf(*saveptr,"%s %d",name,&howManyLayers)<2){
        printf("not enough arguments to function call: up\n");
        return;
    }
    if(howManyLayers<=0){
        printf("the layer shift must be greater than 0\n");
        return;
    }
    el = get_element(c->scene,name);
    if(el){
        up_element(c->scene,el,howManyLayers);
        clear_screen(c->scene);
        draw_scene(c->scene);
        return;
    }
    printf("the element with that name does does not exists in the scene\n");
}
void bottom_command(FILE *fp, char **saveptr, struct context *c){
    if(!c) return;
    char name[32] = {'\0'};
    struct element *el = NULL;
    if(sscanf(*saveptr,"%s",name)<1){
        printf("not enough arguments to function call: bottom\n");
        return;
    }
    el = get_element(c->scene,name);
    if(el){
        bottom_element(c->scene,el);
        clear_screen(c->scene);
        draw_scene(c->scene);
        return;
    }
    printf("the element with that name does does not exists in the scene\n");
}
void top_command(FILE *fp, char **saveptr, struct context *c){
    if(!c) return;
    char name[32] = {'\0'};
    struct element *el = NULL;
    if(sscanf(*saveptr,"%s",name)<1){
        printf("not enough arguments to function call: bottom\n");
        return;
    }
    el = get_element(c->scene,name);
    if(el){
        top_element(c->scene,el);
        clear_screen(c->scene);
        draw_scene(c->scene);
        return;
    }
    printf("the element with that name does does not exists in the scene\n");
}
void list_command(FILE *fp, char **saveptr, struct context *c){
    if(!c) return;
    list_elements(c->scene);
    clear_screen(c->scene);
    draw_scene(c->scene);
}
void write_png_command(FILE *fp, char **saveptr, struct context *c){
    char *token = NULL;
    char *filename = NULL;
    struct params p;
    p.bg_r = 0;
    p.bg_g = 0;
    p.bg_b = 0;
    p.bg_a = 255;
    p.fg_r = 255;
    p.fg_g = 255;
    p.fg_b = 255;
    p.fg_a = 255;
    while(token=strtok_r(NULL," \n\t",saveptr)){
        if(strcmp(token,"-bg")==0){
            if(sscanf(*saveptr,"%hhd %hhd %hhd %hhd",&p.bg_r,&p.bg_g,&p.bg_b,&p.bg_a)<4){
                printf("Not enough arguments given to select background color\n");
                return;
            }
        }
        else if(strcmp(token,"-fg")==0){
            if(sscanf(*saveptr,"%hhd %hhd %hhd %hhd",&p.fg_r,&p.fg_g,&p.fg_b,&p.fg_a)<4){
                printf("Not enough arguments given to select foreground color\n");
                return;
            }
        }
        else if(strcmp(token,"-o")==0){
            filename = *saveptr;
        }
    }
    if(!filename){
        printf("Filename was not given\n");
        return;
    }
    if(!write_png_file(filename,c->scene,&p)) printf("Could not create a png file\n");
    clear_screen(c->scene);
    draw_scene(c->scene);
}
void read_menu(FILE *fp, struct context *c){
    char *token = NULL;
    FILE *handle = NULL;
    char *saveptr = NULL;
    memset(c->line,0,c->linelen);
    while(getline(&c->line,&c->linelen,fp)!=EOF){
        token = strtok_r(c->line," \n\t",&saveptr);
        if(token){
            if(strcmp(token,"load")==0){
                load_command(fp,&saveptr,c);
            }
            else if(strcmp(token,"end")==0){
                end_command(fp,NULL,c);
            }
            else printf("Wrong command\n");
            memset(c->line,0,c->linelen);
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
                            fclose(fp);
                            free(line);
                            printf("file error: did not specify a name for the drawing\n");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                else if(strcmp(token,"width")==0){
                    token = strtok(NULL," \n\t");
                    if(token!=NULL){
                        if(sscanf(token,"%d",&scene_width)==0){
                            fclose(fp);
                            free(line);
                            printf("file error: did not specify correct width value\n");
                            exit(EXIT_FAILURE);
                        }
                        scene_width = scene_width>128?128:scene_width;
                        content_width = scene_width;
                        read_width = true;
                    }
                    else{
                        fclose(fp);
                        free(line);
                        printf("file error: did not specify any width value\n");
                        exit(EXIT_FAILURE);
                    }
                }
                else if(strcmp(token,"height")==0){
                    token = strtok(NULL," \n\t");
                    if(token!=NULL){
                        if(sscanf(token,"%d",&scene_height)==0){
                            fclose(fp);
                            free(line);
                            printf("file error: did not specify correct height value\n");
                            exit(EXIT_FAILURE);
                        }
                        read_height = true;
                    }
                    else{
                        fclose(fp);
                        free(line);
                        printf("file error: did not specify any height value\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
        memset(line,0,linelen);
    }
    fclose(fp);
    free(line);
    change_state(c,stdin,read_drawing);
}
void read_drawing(FILE *fp, struct context *c){
    char *token = NULL;
    int x;
    int y;
    char *saveptr = NULL;
    memset(c->line,0,c->linelen);
    while(getline(&c->line,&c->linelen,fp)!=EOF){
        if(strlen(c->line)>1){
            token = strtok_r(c->line," \n\t",&saveptr);
            if(strcmp(token,"draw")==0) draw_command(fp,&saveptr,c);
            else if(strcmp(token,"delete")==0) delete_command(fp,&saveptr,c);
            else if(strcmp(token,"move")==0) move_command(fp,&saveptr,c);
            else if(strcmp(token,"down")==0) down_command(fp,&saveptr,c);
            else if(strcmp(token,"up")==0) up_command(fp,&saveptr,c);
            else if(strcmp(token,"bottom")==0) bottom_command(fp,&saveptr,c);
            else if(strcmp(token,"top")==0) top_command(fp,&saveptr,c);
            else if(strcmp(token,"list")==0) list_command(fp,&saveptr,c);
            else if(strcmp(token,"png")==0) write_png_command(fp,&saveptr,c);
            else if(strcmp(token,"menu")==0) menu_command(fp,&saveptr,c);
            else if(strcmp(token,"end")==0) end_command(fp,&saveptr,c);
            else printf("Wrong command\n");
            saveptr = NULL;
        }
        memset(c->line,0,c->linelen);
    }
}
struct context *new_context(){
    struct context *c = (struct context*)malloc(sizeof(struct context));
    c->line = (char*)calloc(128,sizeof(char));
    c->linelen = 128;
    c->palette = NULL;
    c->read = NULL;
    c->scene = NULL;
    return c;
}
void destroy_context(struct context *c){
    if(!c) return;
    if(c->line) free(c->line);
    if(c->scene) destroy_scene(c->scene);
    if(c->palette) destroy_palette(c->palette);
    free(c);
}
bool change_state(struct context *c, FILE *fp, void(*read)(FILE*,struct context*)){
    if(!(c && read)) return false;
    c->read = read;
    c->read(fp,c);
    return true;
}