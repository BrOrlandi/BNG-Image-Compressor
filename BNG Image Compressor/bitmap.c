#include "bitmap.h"


void BMPData_init(BMPData *bmp, FILE *image) {

    fread(&(bmp->signature), 1, 2, image);
    fread(&(bmp->file_size), 1, 4, image);
    fread(&(bmp->reserved), 1, 4, image);
    fread(&(bmp->padding), 1, 4, image);

    fread(&(bmp->header_size), 1, 4, image);
    fread(&(bmp->img_width), 1, 4, image);
    fread(&(bmp->img_height), 1, 4, image);
    fread(&(bmp->planes), 1, 2, image);
    fread(&(bmp->pixel_depth), 1, 2, image);
    fread(&(bmp->compression), 1, 4, image);
    fread(&(bmp->bmp_size), 1, 4, image);
    fread(&(bmp->ppm_x), 1, 4, image);
    fread(&(bmp->ppm_y), 1, 4, image);
    fread(&(bmp->color_used), 1, 4, image);
    fread(&(bmp->color_imp), 1, 4, image);

    unsigned int file_size = bmp->file_size;
    unsigned int padding = bmp->padding;

    unsigned int width = bmp->img_width;
    unsigned int height = bmp->img_height;

    // Theoretically, trash amout should be zero, but sometimes its not
    bmp->trash_amount = file_size - ((height * width * 3) + padding);

    // Allocates memory to the whole bitmap
    // The constant 3 indicates the three colors we're working on
    bmp->data = calloc(3*width*height, sizeof(char));
}

void BMPData_destroy(BMPData *bmp) {
    free(bmp->data);
}
