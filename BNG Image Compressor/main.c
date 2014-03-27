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
    BMPData_print(&bitmap);
    BMPData_destroy(&bitmap);

    // Exemplo escrita bit a bit
    BitWriter writer;

    BitWriter_init(&writer, "test.txt");
    BitWriter_write_bits(&writer, 'B', 8);

    // ASCII for N
    BitWriter_write_bit(&writer, 0);
    BitWriter_write_bit(&writer, 1);
    BitWriter_write_bit(&writer, 0);
    BitWriter_write_bit(&writer, 0);
    BitWriter_write_bit(&writer, 1);
    BitWriter_write_bit(&writer, 1);
    BitWriter_write_bit(&writer, 1);
    BitWriter_write_bit(&writer, 0);

    BitWriter_write_bits(&writer, 'G', 8);

    // ASCII for 0 with 4 missing zeros at the end
    BitWriter_write_bit(&writer, 0);
    BitWriter_write_bit(&writer, 0);
    BitWriter_write_bit(&writer, 1);
    BitWriter_write_bit(&writer, 1);

    // If it is alright it will make a test.txt file with PNG0
    // The aditinal 0 in the file comes from the padded zeros (a file must have
    // a byte multiple size).
    BitWriter_close(&writer);

    return 0;
}
