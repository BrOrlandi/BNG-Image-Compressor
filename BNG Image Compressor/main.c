#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"
#include "huffman/huffman.h"
#include "zigzag_vector.h"
#include "run_len.h"
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
    if(!strcmp(argv[2],"-d"))  // modo descompressão
    {
        decompress = 1;
        if(argc < 4)
        {
            printf("Missing <output file>\n");
            return 0;
        }
        outputFile = argv[3];
    }
    else  // modo compressão
    {
        outputFile = argv[2];
    }

    Huffman h;

    Huffman_init(&h);

    if(!decompress)  // compressão
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

        BMPData_init(&bitmap, image); // inicializa variábeis BMP e separa em blocos

        //CODIGO USADO PARA TESTAR VECTORIZE
        int i;
        for (i=0; i<(bitmap.dataSize); i++)
        {
            //if(i%24 == 0)
                //printf("\n");
            //printf("%d ",bitmap.data[i]);
        }
        unsigned char *vector = vectorize(&bitmap); // vetorização de CADA bloco
        unsigned char *blocks = unvectorize(vector, bitmap.img_width*bitmap.img_height*3);

        printf("\n\n\n");
        for (i=0; i<(bitmap.dataSize); i++)
        {
            if (bitmap.data[i] != blocks[i])
            {
                printf("errado");
            }

            //if(i%24 == 0)
                //printf("\n");
            //printf("%d ",blocks[i]);
        }
        //-----------------------------

        //unsigned char *vector = vectorize(&bitmap); // vetorização de CADA bloco

        /*
        unsigned char *blocks = unvectorize(vector, bitmap.dataSize);

        int i;
        for(i = 0; i < bitmap.dataSize; i++) {
            if(blocks[i] != bitmap.block_data[i]) {
                printf("%d: (%d) != (%d)\n", i, blocks[i], bitmap.block_data[i]);
            }
        }
        //        free(blocks);
        //        free(vector);
        /*
        BMPData_print(&bitmap);
        printf("\n--\n");
        BMPData_print_block(&bitmap,0,0);
        printf("\n--\n");
        BMPData_print_block(&bitmap,0,1);
        printf("\n--\n");
        BMPData_print_block(&bitmap,0,2);
        printf("\n--\n");
        */
        unsigned int size;
        unsigned char *data; // the data read from the file
        //data = bitmap.block_data;
        //size = bitmap.img_width*bitmap.img_height*3;

        // Codificação por carreira
        //data = RLE_encode(bitmap.block_data, bitmap.img_width, bitmap.img_height, &size);

        /*int i;
        printf("\n\n\n antes");
        for (i=0; i<(bitmap.dataSize); i++)
        {
            //if(i%24 == 0)
                //printf("\n");
            printf("%d ",vector[i]);
        }

        data = RLE_encode(vector, bitmap.img_width, bitmap.img_height, &size);

        printf("\n\n\n compr");
        for (i=0; i<size; i++)
        {
            //if(i%24 == 0)
                //printf("\n");
            printf("%d ",data[i]);
        }

        int width, height;
        unsigned char *decoded = RLE_decode(vector, &width, &height);

        printf("\n\n\n descom");
        for (i=0; i<(bitmap.dataSize); i++)
        {
            //if(i%24 == 0)
                //printf("\n");
            printf("%d ",decoded[i]);
        }*/


        data = RLE_encode(vector, bitmap.img_width, bitmap.img_height, &size);

        //printf("bitmap.img_width = %d\n",bitmap.img_width);
        //printf("bitmap.img_height = %d\n",bitmap.img_height);
        //printf("decoded size = %d\n",bitmap.img_width*bitmap.img_height*3);
        //printf("encoded size = %d\n",size);

        /*
                int i;
                printf("\n\n");
                for(i=0;i<size;i++){
                    printf("%d ", data[i]);
                }
                printf("\n\n");

                unsigned int w, h;
                unsigned char *decoded = RLE_decode(data,&w,&h);
                printf("w = %d\n",w);
                printf("h = %d\n",h);
                int s = w*h*3;
                for(i=0;i<s;i++){
                    printf("%d ", decoded[i]);
                }
                printf("\n\n");
        //*/

//        int i = 0;
//        for(i = 0; i < bitmap.img_width*bitmap.img_height*3; i++) {
//            if(bitmap.block_data[i] == 0)
//                bitmap.block_data[i]++;
//            if(decoded[i] != bitmap.block_data[i]) {
//                printf("%d: (%d) != (%d)\n", i, decoded[i], bitmap.block_data[i]);
//           }
//        }

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

        BMPData bitmap;
        //BMPData_from_raw(&bitmap, decoded, width, height);
        BMPData_from_raw(&bitmap, blocks, width, height);
        //BMPData_print(&bitmap);

        unsigned char *file_data = calloc(bitmap.file_size, sizeof(unsigned char));
        BMPData_HeaderToChar(&bitmap, file_data);

        // Copia os dados do bitmap para o buffer que será usado para saída
        for(i = 0; i < bitmap.dataSize; i++)
        {
            file_data[i+54] = bitmap.data[i];
        }


        fileWrite(outputFile, file_data, bitmap.file_size);
        //fileWrite(outputFile, h.uc_data[0], h.uc_sizes[0]);
        printf("Image decompressed to: %s\n",outputFile);
    }
    return 0;
}
