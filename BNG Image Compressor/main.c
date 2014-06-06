#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"
#include "huffman/huffman.h"
#include "zigzag_vector.h"
#include "run_len.h"
#include "dctq.h"
#include <string.h>


int main(int argc, char* argv[])
{

    if(argc < 3)  // the file must be in the arguments
    {
        printf("Usage: <input file> [optional -d] <output file>\n\t -d\tThe input file will be decompressed to output file.");
        return 0;
    }

    char *inputFile = argv[1];
    char *outputFile;

    char decompress = 0;
    if(!strcmp(argv[2],"-d"))  // modo descompress�o
    {
        decompress = 1;
        if(argc < 4)
        {
            printf("Missing <output file>\n");
            return 0;
        }
        outputFile = argv[3];
    }
    else  // modo compress�o
    {
        outputFile = argv[2];
    }

    Huffman h;

    Huffman_init(&h);

    if(!decompress)  // compress�o
    {

        FILE *image;
        BMPData bitmap;

        image = fopen(inputFile, "rb");
        if(image == NULL)
        {
            printf("File not found: %s\n",inputFile);
            exit(1);
        }
        printf("Compressing: %s\n", inputFile);

        BMPData_init(&bitmap, image); // inicializa vari�beis BMP e separa em blocos

        unsigned char *dctq = dct(&bitmap);


        unsigned char *vector = vectorize(&bitmap, dctq); // vetoriza��o de CADA bloco

        unsigned int size;
        unsigned char *data; // para armazena a compress�o por carreira

        // Codifica��o por carreira
        data = RLE_encode(vector, bitmap.img_width, bitmap.img_height, &size);


        Huffman_add_data_block(&h,data,size);

        Huffman_apply(&h);

        Huffman_compress_data_to_file(&h,outputFile);
        printf("Image compressed to: %s\n",outputFile);

        BMPData_destroy(&bitmap);
        //free(data);
//*/
    }
    else  // otherwise, the file will be decompressed in a bng file
    {
        printf("Decompressing: %s\n", inputFile);

        unsigned char *data; // new data after decompress

        Huffman_file_decompress(&h,inputFile);

        data = h.uc_data[0];

        int i = 0;
        int width, height;

        // Decode run_length encoded image
        unsigned char *decoded = RLE_decode(data, &width, &height);

        unsigned char *blocks = unvectorize(decoded, width*height*3);

        unsigned char *idctq = idct(blocks, width*height*3);

        BMPData bitmap;
        BMPData_from_raw(&bitmap, idctq, width, height);
        //BMPData_print(&bitmap);

        unsigned char *file_data = calloc(bitmap.file_size, sizeof(unsigned char));
        BMPData_HeaderToChar(&bitmap, file_data);

        // Copia os dados do bitmap para o buffer que ser� usado para sa�da
        for(i = 0; i < bitmap.dataSize; i++)
        {
            file_data[i+54] = bitmap.data[i];
        }


        fileWrite(outputFile, file_data, bitmap.file_size);
        printf("Image decompressed to: %s\n",outputFile);
    }
    return 0;
}
