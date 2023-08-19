#ifndef COMMANDS_H_
#define COMMANDS_H_
#include <programstate.h>

bool load_command(FILE *fp, char **saveptr, struct context *c);
bool continue_command(FILE *fp, char **saveptr, struct context *c);
bool persist_command(FILE *fp, char **saveptr, struct context *c);
bool draw_command(FILE *fp, char **saveptr, struct context *c);
bool line_command(FILE *fp, char **saveptr, struct context *c);
bool circle_command(FILE *fp, char **saveptr, struct context *c);
bool delete_command(FILE *fp, char **saveptr, struct context *c);
bool menu_command(FILE *fp, char **saveptr, struct context *c);
bool end_command(FILE *fp, char **saveptr, struct context *c);
bool move_command(FILE *fp, char **saveptr, struct context *c);
bool down_command(FILE *fp, char **saveptr, struct context *c);
bool up_command(FILE *fp, char **saveptr, struct context *c);
bool bottom_command(FILE *fp, char **saveptr, struct context *c);
bool top_command(FILE *fp, char **saveptr, struct context *c);
bool list_command(FILE *fp, char **saveptr, struct context *c);
bool show_command(FILE *fp, char **saveptr, struct context *c);
bool reverse_command(FILE *fp, char **saveptr, struct context *c);
bool write_png_command(FILE *fp, char **saveptr, struct context *c);

#endif