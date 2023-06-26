#ifndef PALETTE_H_
#define PALETTE_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <content.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct drawing{
    char *name;
    char **content;
    int content_height;
    int content_width;
    struct drawing *left;
    struct drawing *right;
    uint8_t height;
};

struct palette{
    struct drawing *root;
};

void destroy_tree(struct drawing *d);
struct palette *new_palette();
void destroy_palette(struct palette *p);
struct drawing *new_drawing(char *name, char **content, int content_height, int content_width);
bool add_drawing(struct palette *p, struct drawing *d);
struct drawing *get_drawing(struct palette *p, char *name);

#endif