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

void fill_bg(png_bytep *row_pointers, 
    int width, 
    int height, 
    struct params *p);
bool render_character(SFT *instance,
    SFT_UChar codepoint,
    SFT_Glyph *glyph_id,
    SFT_Image image);
bool put_character(png_bytep *row_pointers, 
    int width, 
    int height, 
    int start_x,
    int start_y,
    struct params *p, 
    unsigned char *bitmap,
    int bitmap_width,
    int bitmap_height);
bool write_png_file(char *filename, 
    struct scene *s, 
    struct params *p);

#endif