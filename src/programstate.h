#ifndef PROGRAM_STATE_H_
#define PROGRAM_STATE_H_
#include <stdio.h>
#include <scene.h>
#include <palette.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <content.h>
#include <png.h>
#include <topng.h>
#include <stdint.h>

//menu
//parsing file
//drawing

#pragma pack(1)
struct flags{
    uint8_t read_width:2;
    uint8_t read_height:2;
    uint8_t is_reading_shape:2;
    uint8_t fill:2;
};
#pragma pack()

struct context{
    char *line;
    char *source;
    size_t linelen;
    struct scene *scene;
    struct palette *palette;
    struct content_list *c_list;
    void(*read)(FILE*,struct context*);
};

void read_menu(FILE *fp, struct context *c);
void read_parsing(FILE *fp, struct context *c);
void read_drawing(FILE *fp, struct context *c);
struct context *new_context();
void destroy_context(struct context *c);
bool change_state(struct context *c, FILE *fp, void(*read)(FILE*,struct context*));

#endif