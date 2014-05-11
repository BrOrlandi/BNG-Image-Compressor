#include "bitmap.h"


int block_offset(int index, int x, int y, int color) {
    return (index*8*8*3 + x*8*3 + y*3 + color);
}

int matrix_offset(BMPData *bmp, int x, int y, int color) {
    int width = bmp->img_width;
    return (x*width*3 + y*3 + color);
}

void BMPData_init(BMPData *bmp, FILE *image){

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
    bmp->dataSize = (3*width*height) + bmp->trash_amount;
    bmp->data = calloc(bmp->dataSize, sizeof(char));
    bmp->block_data = (unsigned char*) calloc(bmp->dataSize, sizeof(unsigned char));

    int i = 0;
    char data_block;
    // Populating the matrix
    while(fread(&data_block, 1, sizeof(char), image)) {
        bmp->data[i] = data_block;
        i++;
    }

    BMPData_blockify(bmp);
}

void BMPData_blockify(BMPData *bmp) {
    int i, j, k, x, y;
    int index = 0;

    int width = bmp->img_width;
    int height = bmp->img_height;

    for(y = 0; y < width; y += 8)
    for(x = 0; x < height; x += 8, index++)
    for(i = 0; i < 8; i++) {
        for(j = 0; j < 8; j++) {
            for(k = 0; k < 3; k++) {
                unsigned char value = bmp->data[matrix_offset(bmp, x+i, y+j, k)];
                bmp->block_data[block_offset(index, i, j, k)] = value;
            }
        }
    }
    bmp->block_data_length = width*height/64;
}

void BMPData_unblockify(BMPData *bmp) {
    int i, j, k, x, y;
    int index = 0;

    int width = bmp->img_width;
    int height = bmp->img_height;

    for(y = 0; y < width; y += 8)
    for(x = 0; x < height; x += 8, index++)
    for(i = 0; i < 8; i++) {
        for(j = 0; j < 8; j++) {
            for(k = 0; k < 3; k++) {
                unsigned char value = bmp->block_data[block_offset(index, i, j, k)];
                bmp->data[matrix_offset(bmp, x+i, y+j, k)] = value;
            }
        }
    }
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

void BMPData_print_block(BMPData *bmp, int index, int color) {
    int i, j;
    for(i = 0; i < 8; i++) {
        for(j = 0; j < 8; j++) {
            printf("%d ", bmp->block_data[block_offset(index, i, j, color)]);
        }
        printf("\n");
    }
    printf("\n");
}

void BMPData_destroy(BMPData *bmp) {
    free(bmp->data);
    free(bmp->block_data);
}

void BMPData_from_raw(BMPData *bmp, unsigned char *raw, int width, int height) {

    // Reconstructing header
    bmp->signature = 0x4d42;
    bmp->file_size = (width * height * 3) + 54;
    bmp->reserved = 0;
    bmp->padding = 54;

    bmp->header_size = 40;
    bmp->img_width = width;
    bmp->img_height = height;
    bmp->planes = 2621441;
    bmp->pixel_depth = 2621464;
    bmp->compression = 0;
    bmp->bmp_size = width * height * 3;
    bmp->dataSize = width * height * 3;
    bmp->ppm_x = 0;
    bmp->ppm_y = 0;
    bmp->color_used = 0;
    bmp->color_imp = 0;

    bmp->data = calloc(bmp->dataSize, sizeof(char));
    bmp->block_data = raw;

    BMPData_unblockify(bmp);
}

void BMPData_HeaderToChar(BMPData *bmp, unsigned char *data){
    data[0] = (bmp->signature) & 0xFF;
    data[1] = (bmp->signature >> 8) & 0xFF;

    data[2] = (bmp->file_size) & 0xFF;
    data[3] = (bmp->file_size >> 8) & 0xFF;
    data[4] = (bmp->file_size >> 16) & 0xFF;
    data[5] = (bmp->file_size >> 24) & 0xFF;

    data[6] = (bmp->reserved) & 0xFF;
    data[7] = (bmp->reserved >> 8) & 0xFF;
    data[8] = (bmp->reserved >> 16) & 0xFF;
    data[9] = (bmp->reserved >> 24) & 0xFF;

    data[10] = (bmp->padding) & 0xFF;
    data[11] = (bmp->padding >> 8) & 0xFF;
    data[12] = (bmp->padding >> 16) & 0xFF;
    data[13] = (bmp->padding >> 24) & 0xFF;

    data[14] = (bmp->header_size) & 0xFF;
    data[15] = (bmp->header_size >> 8) & 0xFF;
    data[16] = (bmp->header_size >> 16) & 0xFF;
    data[17] = (bmp->header_size >> 24) & 0xFF;

    data[18] = (bmp->img_width) & 0xFF;
    data[19] = (bmp->img_width >> 8) & 0xFF;
    data[20] = (bmp->img_width >> 16) & 0xFF;
    data[21] = (bmp->img_width >> 24) & 0xFF;

    data[22] = (bmp->img_height) & 0xFF;
    data[23] = (bmp->img_height >> 8) & 0xFF;
    data[24] = (bmp->img_height >> 16) & 0xFF;
    data[25] = (bmp->img_height >> 24) & 0xFF;

    data[26] = (bmp->planes) & 0xFF;
    data[27] = (bmp->planes >> 8) & 0xFF;

    data[28] = (bmp->pixel_depth) & 0xFF;
    data[29] = (bmp->pixel_depth >> 8) & 0xFF;

    data[30] = (bmp->compression) & 0xFF;
    data[31] = (bmp->compression >> 8) & 0xFF;
    data[32] = (bmp->compression >> 16) & 0xFF;
    data[33] = (bmp->compression >> 24) & 0xFF;

    data[34] = (bmp->bmp_size) & 0xFF;
    data[35] = (bmp->bmp_size >> 8) & 0xFF;
    data[36] = (bmp->bmp_size >> 16) & 0xFF;
    data[37] = (bmp->bmp_size >> 24) & 0xFF;

    data[38] = (bmp->ppm_x) & 0xFF;
    data[39] = (bmp->ppm_x >> 8) & 0xFF;
    data[40] = (bmp->ppm_x >> 16) & 0xFF;
    data[41] = (bmp->ppm_x >> 24) & 0xFF;

    data[42] = (bmp->ppm_y) & 0xFF;
    data[43] = (bmp->ppm_y >> 8) & 0xFF;
    data[44] = (bmp->ppm_y >> 16) & 0xFF;
    data[45] = (bmp->ppm_y >> 24) & 0xFF;

    data[46] = (bmp->color_used) & 0xFF;
    data[47] = (bmp->color_used >> 8) & 0xFF;
    data[48] = (bmp->color_used >> 16) & 0xFF;
    data[49] = (bmp->color_used >> 24) & 0xFF;

    data[50] = (bmp->color_imp) & 0xFF;
    data[51] = (bmp->color_imp >> 8) & 0xFF;
    data[52] = (bmp->color_imp >> 16) & 0xFF;
    data[53] = (bmp->color_imp >> 24) & 0xFF;
    return;
}
