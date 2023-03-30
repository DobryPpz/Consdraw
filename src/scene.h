#ifndef SCENE_H_
#define SCENE_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct element{
    char *id;
    int x;
    int y;
    int content_height;
    int content_width;
    char **content;
    struct element *next;
};

struct scene{
    int width;
    int height;
    struct element *head;
    struct element *tail;
    char **canvas;
};

struct scene *new_scene(int width, int height);
struct element *new_element(char *id, int x, int y, int content_height, int content_width, char **content);
struct element *get_element(struct scene *s, char *id);
bool add_to_scene(struct scene *s, struct element *el);
bool remove_from_scene(struct scene *s, char *id);
void destroy_scene(struct scene *s);
void load_scene(struct scene *s);
void draw_scene(struct scene *s);
void clear_canvas(struct scene *s);
void clear_scene(struct scene *s);
void clear_screen(struct scene *s);
void move_element(struct element *el, int x, int y);
void down_element(struct scene *s, struct element *el, int howManyLayers);
void up_element(struct scene *s, struct element *el, int howManyLayers);

#endif