#ifndef CONTENT_H_
#define CONTENT_H_
#include <stdlib.h>
#include <stdbool.h>

struct content_node{
    int content_width;
    int content_height;
    char **content;
    struct content_node *next;
};

struct content_list{
    struct content_node *head;
};

struct content_list *new_content_list();
struct content_node *new_content_node(char **content, int content_width, int content_height);
void destroy_content_list(struct content_list *l);
void destroy_content_node(struct content_node *n);
bool add_content(struct content_list *l, struct content_node *n);

#endif