#include <topng.h>

void cleanup(png_bytep *row_pointers,
    int height,
    SFT_Font *font,
    unsigned char *pixels){
        for(int i=0;i<height;i++){
            free(row_pointers[i]);
        }
        free(row_pointers);
        sft_freefont(font);
        free(pixels);
}
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
bool render_character(SFT *instance,
    SFT_UChar codepoint,
    SFT_Glyph glyph_id,
    SFT_Image image){
        if(sft_lookup(instance,codepoint,&glyph_id)){
            printf("Could not find character\n");
            return false;
        }
        if(sft_render(instance,glyph_id,image)){
            printf("Could not render character\n");
            return false;
        }
        for(int i=0;i<image.height;i++){
            for(int j=0;j<image.width;j++){
                unsigned char *pixel = (unsigned char*)image.pixels+i*image.height+j;
                *pixel = *pixel>0?1:0;
            }
            printf("\n");
        }
        return true;
}
bool put_character(png_bytep *row_pointers, 
    int width, 
    int height, 
    int start_x,
    int start_y,
    struct params *p, 
    SFT_Image image){
        for(int i=0;i<image.height;i++){
            png_bytep row = row_pointers[start_y+i];
            for(int j=0;j<image.width;j++){
                png_bytep px = &(row[start_x+4*j]);
                if(((unsigned char*)image.pixels)[image.height*i+j]){
                    px[0] = p->fg_r;
                    px[1] = p->fg_g;
                    px[2] = p->fg_b;
                    px[3] = p->fg_a;
                }
            }
        }
        return true;
}
bool write_png_file(char *filename, struct scene *s, struct params *p){
    // Initialization of png "canvas"(row_pointers) and filling it with bg color
    int width = s->width*16;
    int height = s->height*16;
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
    instance.xScale = 20;
    instance.yScale = 20;
    instance.xOffset = 0;
    instance.yOffset = 0;
    instance.flags = SFT_DOWNWARD_Y;
    image.height = 16;
    image.width = 16;
    image.pixels = (unsigned char*)calloc(image.width*image.height,sizeof(unsigned char));
    if(!s->canvas){
        printf("The canvas was not created\n");
        return false;
    }
    for(int i=0;i<s->height;i++){
        for(int j=0;j<s->width;j++){
            if(s->canvas[i][j] != ' '){
                if(!render_character(&instance,s->canvas[i][j],glyph_id,image)){
                    printf("Could not render the character\n");
                    cleanup(row_pointers,height,font,image.pixels);
                    return false;
                }
                if(!put_character(row_pointers,width,height,4*image.width*j,image.height*i,p,image)){
                    printf("Could not put the character\n");
                    cleanup(row_pointers,height,font,image.pixels);
                    return false;
                }
            }
        }
    }
    // initializing png writing structures and writing the actual png file
    FILE *fp = fopen(filename,"wb");
    if(!fp){
        cleanup(row_pointers,height,font,image.pixels);
        return false;
    }
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
    if(!png){
        cleanup(row_pointers,height,font,image.pixels);
        return false;
    }
    png_infop info = png_create_info_struct(png);
    if(!info){
        cleanup(row_pointers,height,font,image.pixels);
        return false;
    }
    png_init_io(png,fp);
    png_set_IHDR(
        png,
        info,
        width,
        height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png,info);
    png_write_image(png,row_pointers);
    png_write_end(png,NULL);
    fclose(fp);
    png_destroy_write_struct(&png,&info);
    cleanup(row_pointers,height,font,image.pixels);
    return true;
}