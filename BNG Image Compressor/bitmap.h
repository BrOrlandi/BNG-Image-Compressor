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

    // Image blocks
    unsigned char *block_data;

    // Amount of bytes contained in the image in a 24bit bitmap it is WIDTH*HEIGHT*3
    unsigned int dataSize;

    // Number of accessible blocks
    int block_data_length;
} BMPData;
/**
    Returns the offset needed to access block data in a position based
    on a 4-dimensional array (block[index][x][y][color])
*/
int block_offset(int index, int x, int y, int color);

/**
    Returns the offset needed to access image data in a position based
    on a 3-dimensional array (image[x][y][color])
*/
int matrix_offset(BMPData *bmp, int x, int y, int color);

/**
    Initialize the BMP struct loading the header and the data from file
*/
void BMPData_init(BMPData *bmp, FILE *file);

/**
    Separate raw_data into blocks like block[block_data_length][8][8][3]
*/

void BMPData_blockify(BMPData *bmp);

/**
    Join block data back into a single vector representing the whole image
    as a vector
*/

void BMPData_unblockify(BMPData *bmp);

/**
    Print the BMP Header
*/
void BMPData_print(BMPData *bmp);

/**
    Print the block of the specified color and index
*/
void BMPData_print_block(BMPData *bmp, int index, int color);

/**
    Free the memory allocated to the BMP data
*/
void BMPData_destroy(BMPData *bmp);

/**
    Converts the Header data to a char array for file writing.
*/
void BMPData_HeaderToChar(BMPData *bmp, unsigned char *data);

/**
    Builds bitmap data from a raw array of blocks
*/

void BMPData_from_raw(BMPData *bmp, unsigned char *raw, int width, int height);

#endif // BITMAP_H
