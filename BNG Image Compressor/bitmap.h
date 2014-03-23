#include <stdlib.h>
#include <stdio.h>

#ifndef BITMAP_H

#define BITMAP_H

typedef struct BMPData {

    // signature (must be 0x4d42)
    unsigned int signature;
    // File size
    unsigned int file_size;
    // Reserved (must be 0x00000000)
    unsigned int reserved;
    // Header padding till we can reach the data (in this case 54)
    unsigned int padding;

    // Size of this header (in this case 40)
    unsigned int header_size;
    // Image width and height
    unsigned int img_width, img_height;
    // Number of planes
    unsigned int planes;
    // Pixel depth, we're only covering the 24 bit case
    unsigned int pixel_depth;
    // Compression type (We're not covering this)
    unsigned int compression;
    // Bitmap size
    unsigned int bmp_size;
    // Pixel per meters in x and y
    unsigned int ppm_x, ppm_y;
    // Colors used (commonly on 256 color or less bitmaps)
    unsigned int color_used;
    // Same as color_used
    unsigned int color_imp;

    // Just a variable to store the mysterious bitmap overhead
    unsigned int trash_amount;

    // Image raw data
    unsigned char *data;
} BMPData;

void BMPData_init(BMPData *bmp, FILE *file);

void BMPData_print(BMPData *bmp);

void BMPData_destroy(BMPData *bmp);

#endif // BITMAP_H
