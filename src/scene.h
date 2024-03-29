#ifndef SCENE_H_
#define SCENE_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <utilities.h>
#include <content.h>

enum content_type {DRAWING,LINE,CIRCLE};

struct elem_props{
    int x;
    int y;
    int end_x;
    int end_y;
    int radius;
    char paint;
    bool fill;
    char drawing_name[128];
};

struct element{
    char *id;
    struct elem_props props;
    enum content_type type;
    int content_height;
    int content_width;
    int x;
    int y;
    char **content;
    struct element *next;
};

struct stack{
    struct element *top;
};

struct scene{
    int width;
    int height;
    struct element *head;
    struct element *tail;
    char **canvas;
};

struct scene *new_scene(int width, int height);
struct element *new_element(char *id, int x, int y, struct elem_props props, int content_height, int content_width, char **content, enum content_type type);
struct element *get_element(struct scene *s, char *id);
struct element *shift(struct scene *s);
struct stack *new_stack();
bool destroy_stack(struct stack *s);
bool push(struct stack *s, struct element *el);
struct element *pop(struct stack *s);
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
void bottom_element(struct scene *s, struct element *el);
void top_element(struct scene *s, struct element *el);
void list_elements(struct scene *s);
void show_element(struct scene *s, struct element *el);
bool reverse_elements(struct scene *s);
void reset_tail(struct scene *s);

#endif