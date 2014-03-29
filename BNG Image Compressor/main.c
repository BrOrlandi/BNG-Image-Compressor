#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"
#include "bitwriter.h"
#include "huffman.h"
#include "fileio.h"
#include <string.h>


int main(int argc, char* argv[]) {

    if(argc < 2) {
        printf("usage: \t\tbng <file>");
        return 1;
    }

    char *fileName = argv[1]; // name of the file to be compressed or decompressed

    if(endsWith(fileName,".bng")){ // check if the file is an '.bng' file, if it is, decompress

        unsigned int size; // size of the file to be calculated
        unsigned char *data; // new data after decompress

        huffmanFileRead(fileName,&data,&size);

        // here can make others manipulations to the data

        printf("Saving the uncompressed file.\n");
        int len = strlen(fileName);
        fileName[len-4] = '\0';
        fileWrite(fileName,data,size);
        printf("File decompressed: %s\n",fileName);
    }
    else{ // otherwise, the file will be compres8sed in a bng file

        FILE *image;
        BMPData bitmap;

        unsigned int size;
        image = fopen(fileName, "rb");
        unsigned char *data = fileRead(fileName,&size);
        if(image == NULL){
            printf("File not found: %s\n",fileName);
            exit(1);
        }
        BMPData_init(&bitmap, image);
        BMPData_print(&bitmap);


        huffmanFileWrite(fileName,data,size);

        BMPData_destroy(&bitmap);

    }
    return 0;
}
