#ifndef TO_PNG_H_
#define TO_PNG_H_
#include <stdbool.h>
#include <scene.h>
#include <png.h>
#include <schrift.h>

struct params{
    unsigned char fg_r;
    unsigned char fg_g;
    unsigned char fg_b;
    unsigned char fg_a;
    unsigned char bg_r;
    unsigned char bg_g;
    unsigned char bg_b;
    unsigned char bg_a;
};

void fill_bg(png_bytep *row_pointers, int width, int height, struct params *p);
bool write_png_file(char *filename, struct scene *s, struct params *p);

#endif