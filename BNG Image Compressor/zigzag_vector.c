#include "zigzag_vector.h"

unsigned char *vectorize(BMPData *bmp) {
    unsigned char *blocks = bmp->data;
    unsigned char *zigzag = calloc(bmp->dataSize, sizeof(unsigned char));

    int block_length = bmp->block_data_length;
    int i, x, y, color;
    int index = 0;
    int d;

    // Going through every block
    for(i = 0; i < block_length; i++) {
        // Zigzagging each block into the zigzag vector
        // Putting colors in different vectors
        for(color = 0; color < 3; color++) {
            for(d = 1, x = 0, y = 0; (x < 8) && (y < 8); index++) {
                zigzag[index] = blocks[block_offset(i, x, y, color)];
                x += d;
                y -= d;
                if(y < 0) { // Outside top
                    y = 0;
                    d = -d;
                    continue;
                }
                if(x < 0) {
                    d = -d;
                    if(y > 7) { // Outside left bottom
                        y = 7;
                        x += 2;
                        continue;
                    }
                    // Outside left only
                    x = 0;
                    continue;
                }
                if(x > 7) { // Outside right
                    x = 7;
                    y += 2;
                    d = -d;
                    continue;
                }
                if(y > 7) { // Outside bottom
                    x += 2;
                    y = 7;
                    d = -d;
                    continue;
                }
            }
        }
    }
    /*
    for(i = 0; i < bmp->dataSize; i++) {
        if(i % 64 == 0) {
            printf("\n\n");
        }
        printf("%d ", zigzag[i]);
    }
    printf("\n");
    */

    return zigzag;
}

unsigned char *unvectorize(unsigned char *zigzag, int size) {
    unsigned char *blocks = calloc(size, sizeof(unsigned char));

    int block_length = size / (8 * 8 * 3);
    int i, x, y, color;
    int index = 0;
    int d;

    // Going through every block
    for(i = 0; i < block_length; i++) {
        // Zigzagging each block into the zigzag vector
        // Putting colors in different vectors
        for(color = 0; color < 3; color++) {
            for(d = 1, x = 0, y = 0; (x < 8) && (y < 8); index++) {
                blocks[block_offset(i, x, y, color)] = zigzag[index];
                x += d;
                y -= d;
                if(y < 0) { // Outside top
                    y = 0;
                    d = -d;
                    continue;
                }
                if(x < 0) {
                    d = -d;
                    if(y > 7) { // Outside left bottom
                        y = 7;
                        x += 2;
                        continue;
                    }
                    // Outside left only
                    x = 0;
                    continue;
                }
                if(x > 7) { // Outside right
                    x = 7;
                    y += 2;
                    d = -d;
                    continue;
                }
                if(y > 7) { // Outside bottom
                    x += 2;
                    y = 7;
                    d = -d;
                    continue;
                }
            }
        }
    }
    //printf("len -> %d\n", block_length);
    //printf("sz -> %d\n", size);
    //printf("i -> %d\n", index);
    return blocks;
}
