#include <topng.h>

void fill_bg(png_bytep *row_pointers, int width, int height, struct params *p){
    if(!row_pointers) return;
    for(int y=0;y<height;y++){
        png_bytep row = row_pointers[y];
        for(int x=0;x<width;x++){
            png_bytep px = &(row[4*x]);
            px[0] = p->bg_r;
            px[1] = p->bg_g;
            px[2] = p->bg_b;
            px[3] = p->bg_a;
        }
    }
}
bool write_png_file(char *filename, struct scene *s, struct params *p){
    // Initialization of png "canvas"(row_pointers) and filling it with bg color
    int width = s->width*16;
    int height = s->height*16;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep)*height);
    for(int y=0;y<height;y++){
        row_pointers[y] = (png_byte*)malloc(4*width);
    }
    fill_bg(row_pointers,width,height,p);
    // Putting characters on row_pointers
    SFT_Font *font = sft_loadfile("/usr/share/fonts/truetype/freefont/FreeMono.ttf");
    if(!font){
        printf("Could not load the font\n");
        return false;
    }
    SFT_Glyph glyph_id;
    SFT instance;
    SFT_Image image;
    instance.font = font;
    instance.xScale = 27;
    instance.yScale = 27;
    instance.xOffset = 0;
    instance.yOffset = 0;
    instance.flags = SFT_DOWNWARD_Y;
    image.height = 16;
    image.width = 16;
    image.pixels = (unsigned char*)calloc(image.width*image.height,sizeof(unsigned char));
    // writing row_pointers to png

}