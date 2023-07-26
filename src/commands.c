#include <commands.h>

void load_command(FILE *fp, char **saveptr, struct context *c){
    char *token = NULL;
    FILE *handle = NULL;
    token = strtok_r(NULL," \n\t",saveptr);
    int srclen;
    if(!token){
        printf("Did not specify a file\n");
        return;
    }
    handle = fopen(token,"r");
    if(handle){
        srclen = strlen(token);
        c->source = (char*)calloc(srclen+1,sizeof(char));
        sprintf(c->source,"%s",token);
        change_state(c,handle,read_parsing);
    }
    else{
        printf("Could not open the file\n");
    }
}
void continue_command(FILE *fp, char **saveptr, struct context *c){
    struct flags flags;
    c->palette = new_palette();
    c->c_list = new_content_list();
    flags.read_width = false;
    flags.read_height = false;
    flags.is_reading_shape = false;
    FILE *handle = NULL;
    char *token = NULL;
    char **content = NULL;
    char *name = NULL;
    int content_height;
    int content_width;
    int scene_width;
    int scene_height;
    int srclen;
    token = strtok_r(NULL," \n\t",saveptr);
    if(!token){
        printf("Did not specify a file\n");
        return;
    }
    handle = fopen(token,"r");
    if(!handle){
        printf("Could not open the file\n");
        return;
    }
    srclen = strlen(token);
    c->source = (char*)calloc(srclen+1,sizeof(char));
    sprintf(c->source,"%s",token);
    memset(c->line,0,c->linelen);
    while(getline(&c->line,&c->linelen,handle)!=EOF){
        if(memcmp(c->line,"scene",5)==0){
            break;
        }
        if(flags.is_reading_shape){
            if(c->line[0]=='e' && c->line[1]=='n' && c->line[2]=='d'){
                flags.is_reading_shape = false;
                add_content(c->c_list,new_content_node(content,content_width,content_height));
                struct drawing *d = new_drawing(name,content,content_height,content_width);
                add_drawing(c->palette,d);
                name = NULL;
                content = NULL;
                content_height = 0;
            }
            else{
                content_height++;
                c->line[strlen(c->line)-1] = '\0';
                content = (char**)realloc(content,content_height*sizeof(char*));
                content[content_height-1] = (char*)calloc(content_width,sizeof(char));
                sprintf(content[content_height-1],"%s",c->line);
            }
        }
        else{
            token = strtok(c->line," \n\t");
            if(token!=NULL){
                if(strcmp(token,"begin")==0){
                    if(!(flags.read_width && flags.read_height)){
                        printf("file error: did not specify width and height parameters\n");
                        end_command(fp,NULL,c);
                    }
                    else{
                        if(c->scene==NULL){
                            c->scene = new_scene(scene_width,scene_height);
                        }
                        token = strtok(NULL," \n\t");
                        if(token!=NULL){
                            name = (char*)calloc(scene_width,sizeof(char));
                            sscanf(token,"%s",name);
                            flags.is_reading_shape = true;
                            content_height = 0;
                        }
                        else{
                            fclose(fp);
                            free(c->line);
                            printf("file error: did not specify a name for the drawing\n");
                            end_command(fp,NULL,c);
                        }
                    }
                }
                else if(strcmp(token,"width")==0){
                    token = strtok(NULL," \n\t");
                    if(token!=NULL){
                        if(sscanf(token,"%d",&scene_width)==0){
                            fclose(fp);
                            free(c->line);
                            printf("file error: did not specify correct width value\n");
                            end_command(fp,NULL,c);
                        }
                        scene_width = scene_width>128?128:scene_width;
                        content_width = scene_width;
                        flags.read_width = true;
                    }
                    else{
                        fclose(fp);
                        free(c->line);
                        printf("file error: did not specify any width value\n");
                        end_command(fp,NULL,c);
                    }
                }
                else if(strcmp(token,"height")==0){
                    token = strtok(NULL," \n\t");
                    if(token!=NULL){
                        if(sscanf(token,"%d",&scene_height)==0){
                            fclose(fp);
                            free(c->line);
                            printf("file error: did not specify correct height value\n");
                            end_command(fp,NULL,c);
                        }
                        flags.read_height = true;
                    }
                    else{
                        fclose(fp);
                        free(c->line);
                        printf("file error: did not specify any height value\n");
                        end_command(fp,NULL,c);
                    }
                }
            }
        }
        memset(c->line,0,c->linelen);
    }
    if(c->scene==NULL || !flags.read_height || !flags.read_width){
        printf("Couldn't understand the file format\n");
        fclose(handle);
        end_command(NULL,NULL,c);
    }
    change_state(c,handle,read_drawing);
    fclose(handle);
    change_state(c,stdin,read_drawing);
}
void persist_command(FILE *fp, char **saveptr, struct context *c){
    char *name = strtok_r(NULL," \n\t",saveptr);
    FILE *dst = fopen(name,"w");
    FILE *src = fopen(c->source,"r");
    if(dst==NULL || src==NULL){
        printf("error in persist: could not create output file\n");
        return;
    }
    memset(c->line,0,c->linelen);
    while(getline(&c->line,&c->linelen,src)!=EOF){
        if(memcmp(c->line,"scene",5)==0){
            break;
        }
        fputs(c->line,dst);
        memset(c->line,0,c->linelen);
    }
    fputs("\nscene\n",dst);
    if(c->scene->head==NULL){
        printf("there's nothing to persist\n");
        return;
    }
    struct element *traverser = c->scene->head;
    while(traverser!=NULL){
        switch(traverser->type){
            case DRAWING:
                fprintf(dst,"draw %s = %s %d %d\n",traverser->id,
                    traverser->props.drawing_name,
                    traverser->props.x,
                    traverser->props.y);
                break;
            case LINE:
                fprintf(dst,"line %s = %d %d %d %d %c\n",traverser->id,
                    traverser->props.x,
                    traverser->props.y,
                    traverser->props.end_x,
                    traverser->props.end_y,
                    traverser->props.paint);
                break;
            case CIRCLE:
                fprintf(dst,"circle %s = %d %d %d %c %s",traverser->id,
                    traverser->props.x,
                    traverser->props.y,
                    traverser->props.radius,
                    traverser->props.paint,
                    (traverser->props.fill ? "fill\n" : "\n"));
                break;
        }
        traverser = traverser->next;
    }
    fclose(src);
    fclose(dst);
    printf("persisted. press any key\n");
    getchar();
    clear_screen(c->scene);
    draw_scene(c->scene);
}
void draw_command(FILE *fp, char **saveptr, struct context *c){
    char name[32] = {'\0'};
    char relation[32] = {'\0'};
    char drawing[32] = {'\0'};
    int x;
    int y;
    struct elem_props props;
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
    props.x = x;
    props.y = y;
    memset(props.drawing_name,0,128);
    sprintf(props.drawing_name,"%s",drawing);
    el = new_element(name,x,y,props,d->content_height,d->content_width,d->content,DRAWING);
    if(!el) return;
    if(!add_to_scene(c->scene,el)) return;
    clear_screen(c->scene);
    draw_scene(c->scene);
}
void line_command(FILE *fp, char **saveptr, struct context *c){
    char name[32] = {0};
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    char paint;
    struct elem_props props;
    struct element *el = NULL;
    char *token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL){
        printf("name of line was not given\n");
        return;
    }
    memmove(name,token,31);
    el = get_element(c->scene,name);
    if(el){
        printf("the element called like that already exists in the scene %s %s\n",name,el->id);
        return;
    }
    token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL || strcmp(token,"=")!=0){
        printf("wrong relation\n");
        return;
    }
    token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL){
        printf("starting x coordinate was not given\n");
        return;
    }
    sscanf(token,"%d",&start_x);
    if(start_x<0){
        printf("wrong starting x coordinate value\n");
        return;
    }
    token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL){
        printf("starting y coordinate was not given\n");
        return;
    }
    sscanf(token,"%d",&start_y);
    if(start_y<0){
        printf("wrong starting y coordinate value\n");
        return;
    }
    token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL){
        printf("ending x coordinate was not given\n");
        return;
    }
    sscanf(token,"%d",&end_x);
    if(end_x<0){
        printf("wrong ending x coordinate value\n");
        return;
    }
    token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL){
        printf("ending y coordinate was not given\n");
        return;
    }
    sscanf(token,"%d",&end_y);
    if(end_y<0){
        printf("wrong ending y coordinate value\n");
        return;
    }
    token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL){
        printf("line paint was not given\n");
        return;
    }
    paint = token[0];
    int left_x = min_int(start_x,end_x);
    int right_x = max_int(start_x,end_x);
    int up_y = min_int(start_y,end_y);
    int down_y = max_int(start_y,end_y);
    int part_for_y;
    int part_for_x;
    char **content = (char**)calloc(c->scene->height,sizeof(char*));
    if(content==NULL){
        printf("could not allocate enough memory for line content\n");
        return;
    }
    for(int i=0;i<c->scene->height;i++){
        content[i] = (char*)malloc(c->scene->width*sizeof(char));
    }
    for(int i=0;i<c->scene->height;i++){
        for(int j=0;j<c->scene->width;j++){
            if(j>=left_x && j<=right_x && i>=up_y && i<=down_y){
                part_for_y = (i-up_y)*(right_x-left_x);
                part_for_x = (j-left_x)*(down_y-up_y);
                if(abs(part_for_x-part_for_y)<5){
                    content[i][j] = paint;
                }
                else{
                    content[i][j] = ' ';
                }
            }
            else{
                content[i][j] = ' ';
            }
        }
    }
    props.x = start_x;
    props.y = start_y;
    props.end_x = end_x;
    props.end_y = end_y;
    props.paint = paint;
    add_content(c->c_list,new_content_node(content,c->scene->width,c->scene->height));
    el = new_element(name,0,0,props,c->scene->height,c->scene->width,content,LINE);
    if(!el) return;
    if(!add_to_scene(c->scene,el)) return;
    clear_screen(c->scene);
    draw_scene(c->scene);
}
void circle_command(FILE *fp, char **saveptr, struct context *c){
    struct flags flags;
    char name[32] = {0};
    int x;
    int y;
    int radius;
    char paint;
    struct elem_props props;
    char arg[32] = {0};
    flags.fill = false;
    char *token = NULL;
    struct element *el = NULL;
    token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL){
        printf("name of circle was not given\n");
        return;
    }
    memmove(name,token,31);
    el = get_element(c->scene,name);
    if(el){
        printf("the element called like that already exists in the scene %s %s\n",name,el->id);
        return;
    }
    token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL || strcmp(token,"=")!=0){
        printf("wrong relation\n");
        return;
    }
    token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL){
        printf("the x coordinate was not given\n");
        return;
    }
    if(sscanf(token,"%d",&x)==0 || x<0){
        printf("wrong x coordinate\n");
        return;
    }
    token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL){
        printf("the y coordinate was not given\n");
        return;
    }
    if(sscanf(token,"%d",&y)==0 || y<0){
        printf("wrong y coordinate\n");
        return;
    }
    token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL){
        printf("the radius was not given\n");
        return;
    }
    if(sscanf(token,"%d",&radius)==0 || radius<0){
        printf("wrong radius\n");
        return;
    }
    token = strtok_r(NULL," \n\t",saveptr);
    if(token==NULL){
        printf("the circle paint was not given\n");
        return;
    }
    paint = token[0];
    token = strtok_r(NULL," \n\t",saveptr);
    if(token!=NULL){
        memmove(arg,token,31);
        if(strcmp(arg,"fill")==0){
            flags.fill = true;
        }
        else{
            printf("wrong fill argument\n");
            return;
        }
    }
    char **content = (char**)calloc(c->scene->height,sizeof(char*));
    if(content==NULL){
        printf("could not allocate enough memory for circle content\n");
        return;
    }
    for(int i=0;i<c->scene->height;i++){
        content[i] = (char*)malloc(c->scene->width*sizeof(char));
    }
    for(int i=0;i<c->scene->height;i++){
        for(int j=0;j<c->scene->width;j++){
            if(absl(dist(i,j,x,y)-radius)<=1){
                content[i][j] = paint;
            }
            else{
                content[i][j] = ' ';
            }
            if(flags.fill && dist(i,j,x,y)<radius){
                content[i][j] = paint;
            }
        }
    }
    props.x = x;
    props.y = y;
    props.radius = radius;
    props.paint = paint;
    props.fill = flags.fill;
    add_content(c->c_list,new_content_node(content,c->scene->width,c->scene->height));
    el = new_element(name,0,0,props,c->scene->height,c->scene->width,content,CIRCLE);
    if(!el) return;
    if(!add_to_scene(c->scene,el)) return;
    clear_screen(c->scene);
    draw_scene(c->scene);
}
void delete_command(FILE *fp, char **saveptr, struct context *c){
    char name[32] = {'\0'};
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
    printf("the element with that name does not exist in the scene\n");
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
    if(howManyLayers==0){
        printf("the layer shift must not be 0\n");
        return;
    }
    el = get_element(c->scene,name);
    if(!el){
        printf("the element with that name does not exist in the scene\n");
        return;
    }
    if(howManyLayers<0){
        up_element(c->scene,el,-1*howManyLayers);
        clear_screen(c->scene);
        draw_scene(c->scene);
    }
    else{
        down_element(c->scene,el,howManyLayers);
        clear_screen(c->scene);
        draw_scene(c->scene);
    }
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
    if(howManyLayers==0){
        printf("the layer shift must not be 0\n");
        return;
    }
    el = get_element(c->scene,name);
    if(!el){
        printf("the element with that name does not exist in the scene\n");
        return;
    }
    if(howManyLayers<0){
        down_element(c->scene,el,-1*howManyLayers);
        clear_screen(c->scene);
        draw_scene(c->scene);
    }
    else{
        up_element(c->scene,el,howManyLayers);
        clear_screen(c->scene);
        draw_scene(c->scene);
    }
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
    printf("the element with that name does not exist in the scene\n");
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
    printf("the element with that name does not exist in the scene\n");
}
void list_command(FILE *fp, char **saveptr, struct context *c){
    if(!c) return;
    list_elements(c->scene);
    clear_screen(c->scene);
    draw_scene(c->scene);
}
void show_command(FILE *fp, char **saveptr, struct context *c){
    if(!c) return;
    char name[32] = {'\0'};
    struct element *el = NULL;
    if(sscanf(*saveptr,"%s",name)<1){
        printf("not enough arguments to function call: show\n");
        return;
    }
    el = get_element(c->scene,name);
    if(el){
        show_element(c->scene,el);
        clear_screen(c->scene);
        draw_scene(c->scene);
        return;
    }
    printf("the elements with that name does not exist in the scene\n");
}
void reverse_command(FILE *fp, char **saveptr, struct context *c){
    if(!c) return;
    reverse_elements(c->scene);
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
    while((token=strtok_r(NULL," \n\t",saveptr))!=NULL){
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