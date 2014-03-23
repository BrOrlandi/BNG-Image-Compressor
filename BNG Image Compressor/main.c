#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"


int main(int argc, char* argv[]) {

    if(argc < 2) {
        printf("usage: \t\tbng <file>");
        return 1;
    }

    FILE *image;
    BMPData bitmap;

    image = fopen(argv[1], "rb");

    BMPData_init(&bitmap, image);
    BMPData_destroy(&bitmap);

    return 0;
}
