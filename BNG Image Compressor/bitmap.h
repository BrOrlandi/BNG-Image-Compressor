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
    unsigned int dataSize;

    //unsigned char ****blocks; // blocks[index of block][color][x][y]; blocks of 8x8 pixels
    //unsigned int n_blocks; // number of blocks of 8x8 pixels
} BMPData;
/**
    Initialize the BMP struct loading the header and the data from file
*/
void BMPData_init(BMPData *bmp, FILE *file);

/**
    Print the BMP Header
*/
void BMPData_print(BMPData *bmp);

/**
    Returns a pointer to a given index of the block 8x8 pixels, the color(0=B, 1=G, 2=R) and the coordinates X and Y.
*/
unsigned char *BMPData_getp(BMPData *bmp, int index, int color, int x, int y);

/**
    Returns the value of a given index of the block 8x8 pixels, the color(0=B, 1=G, 2=R) and the coordinates X and Y.
*/
unsigned char BMPData_get(BMPData *bmp, int index, int color, int x, int y);

/**
    Set the value to a given index of the block 8x8 pixels, the color(0=B, 1=G, 2=R) and the coordinates X and Y.
*/
void BMPData_set(BMPData *bmp, int index, int color,
                 int x, int y, unsigned char value);

/**
    Free the memory allocated to the BMP data
*/
void BMPData_destroy(BMPData *bmp);

/**
    Converts the Header data to a char array for file writing.
*/
void BMPData_HeaderToChar(BMPData *bmp, unsigned char *data);

#endif // BITMAP_H
