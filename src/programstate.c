#include <programstate.h>
#include <commands.h>

void read_menu(FILE *fp, struct context *c){
    char *token = NULL;
    char *saveptr = NULL;
    memset(c->line,0,c->linelen);
    while(getline(&c->line,&c->linelen,fp)!=EOF){
        token = strtok_r(c->line," \n\t",&saveptr);
        if(token){
            if(strcmp(token,"load")==0) load_command(fp,&saveptr,c);
            else if(strcmp(token,"end")==0) end_command(fp,NULL,c);
            else if(strcmp(token,"continue")==0) continue_command(fp,&saveptr,c);
            else printf("Wrong command\n");
            memset(c->line,0,c->linelen);
        }
    }
}
void read_parsing(FILE *fp, struct context *c){
    c->palette = new_palette();
    c->c_list = new_content_list();
    struct flags flags;
    flags.read_width = false;
    flags.read_height = false;
    flags.is_reading_shape = false;
    char *token = NULL;
    char **content = NULL;
    char *name = NULL;
    int content_height;
    int content_width;
    int scene_width;
    int scene_height;
    while(getline(&c->line,&c->linelen,fp)!=EOF){
        if(flags.is_reading_shape){
            if(c->line[0]=='e' && c->line[1]=='n' && c->line[2]=='d' && flags.is_reading_shape){
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
    fclose(fp);
    if(c->scene==NULL || !flags.read_height || !flags.read_width || flags.is_reading_shape){
        printf("Couldn't understand the file format\n");
        destroy_content(content,content_height);
        if(name!=NULL){
            free(name);
        }
        end_command(NULL,NULL,c);
    }
    change_state(c,stdin,read_drawing);
}
void read_drawing(FILE *fp, struct context *c){
    char *token = NULL;
    char *saveptr = NULL;
    memset(c->line,0,c->linelen);
    while(getline(&c->line,&c->linelen,fp)!=EOF){
        if(strlen(c->line)>1){
            token = strtok_r(c->line," \n\t",&saveptr);
            if(strcmp(token,"draw")==0) draw_command(fp,&saveptr,c);
            else if(strcmp(token,"line")==0) line_command(fp,&saveptr,c);
            else if(strcmp(token,"circle")==0) circle_command(fp,&saveptr,c);
            else if(strcmp(token,"delete")==0) delete_command(fp,&saveptr,c);
            else if(strcmp(token,"move")==0) move_command(fp,&saveptr,c);
            else if(strcmp(token,"down")==0) down_command(fp,&saveptr,c);
            else if(strcmp(token,"up")==0) up_command(fp,&saveptr,c);
            else if(strcmp(token,"bottom")==0) bottom_command(fp,&saveptr,c);
            else if(strcmp(token,"top")==0) top_command(fp,&saveptr,c);
            else if(strcmp(token,"list")==0) list_command(fp,&saveptr,c);
            else if(strcmp(token,"show")==0) show_command(fp,&saveptr,c);
            else if(strcmp(token,"reverse")==0) reverse_command(fp,&saveptr,c);
            else if(strcmp(token,"png")==0) write_png_command(fp,&saveptr,c);
            else if(strcmp(token,"menu")==0) menu_command(fp,&saveptr,c);
            else if(strcmp(token,"end")==0) end_command(fp,&saveptr,c);
            else if(strcmp(token,"persist")==0) persist_command(fp,&saveptr,c);
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
    c->c_list = NULL;
    return c;
}
void destroy_context(struct context *c){
    if(!c) return;
    if(c->line) free(c->line);
    if(c->source) free(c->source);
    if(c->scene) destroy_scene(c->scene);
    if(c->palette) destroy_palette(c->palette);
    if(c->c_list) destroy_content_list(c->c_list);
    free(c);
}
bool change_state(struct context *c, FILE *fp, void(*read)(FILE*,struct context*)){
    if(!(c && read)) return false;
    c->read = read;
    c->read(fp,c);
    return true;
}