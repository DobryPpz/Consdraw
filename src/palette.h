#ifndef PALETTE_H_
#define PALETTE_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <content.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <hashtable.h>

struct drawing{
    char *name;
    char **content;
    int content_height;
    int content_width;
    uint8_t height;
};

struct palette{
    hash_table table;
};

const void *get_drawing_key(void *drawing);
int drawing_cmp(const void *drw1, const void *drw2);
void destroy_drawing(void *drawing);
struct palette *new_palette();
void destroy_palette(struct palette *p);
struct drawing *new_drawing(char *name, char **content, int content_height, int content_width);
bool add_drawing(struct palette *p, struct drawing *d);
struct drawing *get_drawing(struct palette *p, char *name);

#endif