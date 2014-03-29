#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"
#include "bitwriter.h"


int main(int argc, char* argv[]) {

    if(argc < 2) {
        printf("usage: \t\tbng <file>");
        return 1;
    }

    // Exemplo bitmap
    FILE *image;
    BMPData bitmap;

    image = fopen(argv[1], "rb");

    BMPData_init(&bitmap, image);

    unsigned char *dataH;
    unsigned int sizeH;
    BMPData_HeaderToChar(&bitmap, &dataH, &sizeH);

    BMPData_print(&bitmap);



    BMPData_destroy(&bitmap);

    return 0;
}
