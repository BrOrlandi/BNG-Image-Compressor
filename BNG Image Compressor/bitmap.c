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

void BMPData_print(BMPData *bmp) {
    printf("signature:   (x) %x\n", bmp->signature);
    printf("file_size:   (d) %u\n", bmp->file_size);
    printf("reserved:    (x) %x\n", bmp->reserved);
    printf("padding:     (d) %u\n\n", bmp->padding);

    printf("header_size: (d) %u\n", bmp->header_size);
    printf("img_width:   (d) %u\n", bmp->img_width);
    printf("img_height:  (d) %u\n", bmp->img_height);
    printf("planes:      (d) %u\n", bmp->planes);
    printf("pixel_depth: (d) %u\n", bmp->pixel_depth);
    printf("compression: (d) %u\n", bmp->compression);
    printf("bmp_size:    (d) %u\n", bmp->bmp_size);
    printf("ppm_x:       (d) %u\n", bmp->ppm_x);
    printf("ppm_y:       (d) %u\n", bmp->ppm_y);
    printf("color_used:  (d) %u\n", bmp->color_used);
    printf("color_imp:   (d) %u\n\n", bmp->color_imp);

    printf("trash_amount:(d) %u\n", bmp->trash_amount);
}

void BMPData_destroy(BMPData *bmp) {
    free(bmp->data);
}
