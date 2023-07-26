#ifndef COMMANDS_H_
#define COMMANDS_H_
#include <programstate.h>

void load_command(FILE *fp, char **saveptr, struct context *c);
void continue_command(FILE *fp, char **saveptr, struct context *c);
void persist_command(FILE *fp, char **saveptr, struct context *c);
void draw_command(FILE *fp, char **saveptr, struct context *c);
void line_command(FILE *fp, char **saveptr, struct context *c);
void circle_command(FILE *fp, char **saveptr, struct context *c);
void delete_command(FILE *fp, char **saveptr, struct context *c);
void menu_command(FILE *fp, char **saveptr, struct context *c);
void end_command(FILE *fp, char **saveptr, struct context *c);
void move_command(FILE *fp, char **saveptr, struct context *c);
void down_command(FILE *fp, char **saveptr, struct context *c);
void up_command(FILE *fp, char **saveptr, struct context *c);
void bottom_command(FILE *fp, char **saveptr, struct context *c);
void top_command(FILE *fp, char **saveptr, struct context *c);
void list_command(FILE *fp, char **saveptr, struct context *c);
void show_command(FILE *fp, char **saveptr, struct context *c);
void reverse_command(FILE *fp, char **saveptr, struct context *c);
void write_png_command(FILE *fp, char **saveptr, struct context *c);

#endif