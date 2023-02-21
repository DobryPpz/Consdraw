#ifndef PROGRAM_STATE_H_
#define PROGRAM_STATE_H_
#include <stdio.h>
#include <scene.h>
#include <palette.h>
#include <memory.h>

//menu
//parsing file
//drawing

struct context{
    struct scene *scene;
    struct palette *palette;
    void(*read)(FILE*,struct context*);
};

void read_menu(FILE *fp, struct context *c);
void read_parsing(FILE *fp, struct context *c);
void read_drawing(FILE *fp, struct context *c);
struct context *new_context();
void destroy_context(struct context *c);
bool change_state(struct context *c, FILE *fp, void(*read)(FILE*,struct context*));

#endif