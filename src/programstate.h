#ifndef PROGRAM_STATE_H_
#define PROGRAM_STATE_H_
#include <stdio.h>
#include <scene.h>
#include <palette.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//menu
//parsing file
//drawing

struct context{
    struct scene *scene;
    struct palette *palette;
    void(*read)(FILE*,struct context*);
};

void draw_command(FILE *fp, char **saveptr, struct context *c);
void delete_command(FILE *fp, char **saveptr, struct context *c);
void menu_command(FILE *fp, char **saveptr, struct context *c);
void end_command(FILE *fp, char **saveptr, struct context *c);
void move_command(FILE *fp, char **saveptr, struct context *c);
void read_menu(FILE *fp, struct context *c);
void read_parsing(FILE *fp, struct context *c);
void read_drawing(FILE *fp, struct context *c);
struct context *new_context();
void destroy_context(struct context *c);
bool change_state(struct context *c, FILE *fp, void(*read)(FILE*,struct context*));

#endif