#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"
#include "huffman/huffman.h"
#include "zigzag_vector.h"
#include "run_len.h"
#include <string.h>


int main(int argc, char* argv[]) {

    if(argc < 3){ // the file must be in the arguments
        printf("Usage: <input file> [optional -d] <output file>\n\t -d\tThe input file will be decompressed to output file.");
        return 0;
    }

    char *inputFile = argv[1];
    char *outputFile;

    char decompress = 0;
    if(!strcmp(argv[2],"-d")){
        decompress = 1;
        if(argc < 4){
            printf("Missing <output file>\n");
            return 0;
        }
        outputFile = argv[3];
    }
    else{
        outputFile = argv[2];
    }

    Huffman h;

    Huffman_init(&h);

    if(!decompress){

        FILE *image;
        BMPData bitmap;

        image = fopen(inputFile, "rb");
        if(image == NULL){
            printf("File not found: %s\n",inputFile);
            exit(1);
        }
        BMPData_init(&bitmap, image);

//        unsigned char *vector = vectorize(&bitmap);
//        unsigned char *blocks = unvectorize(vector, bitmap.dataSize);
//
//        int i;
//        for(i = 0; i < bitmap.dataSize; i++) {
//            if(blocks[i] != bitmap.block_data[i]) {
//                printf("%d: (%d) != (%d)\n", i, blocks[i], bitmap.block_data[i]);
//            }
//        }
//        free(blocks);
//        free(vector);
        BMPData_print(&bitmap);
        int size;
        unsigned char *data; // the data read from the file

        // Codificação por carreira
        data = encode(bitmap.block_data, bitmap.img_width, bitmap.img_height, &size);
        unsigned char *decoded = decode(data, bitmap.img_width*bitmap.img_height*3);

//        int i = 0;
//        for(i = 0; i < bitmap.img_width*bitmap.img_height*3; i++) {
//            if(bitmap.block_data[i] == 0)
//                bitmap.block_data[i]++;
//            if(decoded[i] != bitmap.block_data[i]) {
//                printf("%d: (%d) != (%d)\n", i, decoded[i], bitmap.block_data[i]);
//           }
//        }

        printf("Compressing: %s\n", inputFile);
        Huffman_add_data_block(&h,data,size);

        Huffman_apply(&h);

        Huffman_compress_data_to_file(&h,outputFile);
        printf("Image compressed to: %s\n",outputFile);

        BMPData_destroy(&bitmap);
        free(data);

    }
    else{ // otherwise, the file will be decompressed in a bng file
        printf("Decompressing: %s\n", inputFile);

        unsigned char *data; // new data after decompress

        Huffman_file_decompress(&h,inputFile);

        data = h.uc_data[0];

        int i = 0;
        int width, height;

        // Recuperando os dados que indicam a altura e largura da imagem
        // Estes dados fora codificados em run_len.c
        width = data[i++];
        width |= (data[i++] & 255) << 8;
        width |= (data[i++] & 255) << 16;
        width |= (data[i++] & 255) << 24;

        height = data[i++];
        height |= (data[i++] & 255) << 8;
        height |= (data[i++] & 255) << 8;
        height |= (data[i++] & 255) << 8;

        // Decode run_length encoded image
        unsigned char *decoded = decode(data, width*height*3);

        BMPData bitmap;
        BMPData_from_raw(&bitmap, decoded, width, height);
        BMPData_print(&bitmap);

        unsigned char *file_data = calloc(bitmap.file_size, sizeof(unsigned char));
        BMPData_HeaderToChar(&bitmap, file_data);

        // Copia os dados do bitmap para o buffer que será usado para saída
        for(i = 0; i < bitmap.dataSize; i++) {
            file_data[i+54] = bitmap.data[i];
        }


        fileWrite(outputFile, file_data, bitmap.file_size);
        printf("Image decompressed to: %s\n",outputFile);
    }
    return 0;
}
