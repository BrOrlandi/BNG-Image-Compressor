#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"
#include "huffman/huffman.h"
#include "zigzag_vector.h"
#include "run_len.h"
#include <string.h>


int main(int argc, char* argv[])
{

    if(argc < 3)  // deve ter arquivos nos argumentos
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

    Huffman_init(&h); // inicializa uma estrutura para trabalhar com Huffman

    if(!decompress)  // compress�o
    {

        FILE *image;
        BMPData bitmap; // estrutura para trabalhar com o bitmap

        image = fopen(inputFile, "rb"); // abrir o arquivo em leitura para iniciar a compress�o
        if(image == NULL)
        {
            printf("File not found: %s\n",inputFile);
            exit(1);
        }
        printf("Compressing: %s\n", inputFile);

        BMPData_init(&bitmap, image); // inicializa vari�veis BMP e separa em blocos

        unsigned char *dctq = dct(&bitmap); // aplica a DCT na imagem


        unsigned char *vector = vectorize(&bitmap, dctq); // vetoriza��o de CADA bloco, utiliza os blocos da DCT, o bitmap contem a quantidade de blocos

        unsigned int size;
        unsigned char *data; // para armazena a compress�o por carreira

        // Codifica��o por carreira
        data = RLE_encode(vector, bitmap.img_width, bitmap.img_height, &size);


        Huffman_add_data_block(&h,data,size); // adiciona o bloco de dados para compress�o de Huffman

        Huffman_apply(&h); // contagem de frequencias, gera��o da arvore de Huffman, gera��o dos c�digos de Huffman

        Huffman_compress_data_to_file(&h,outputFile); // compress�o de Huffman nos dados
        printf("Image compressed to: %s\n",outputFile);

        BMPData_destroy(&bitmap);
    }
    else  // descompress�o
    {
        printf("Decompressing: %s\n", inputFile);

        unsigned char *data;

        Huffman_file_decompress(&h,inputFile); // Decommpress�o de Huffman

        data = h.uc_data[0]; // pega o primeiro bloco de dados do Huffman que s�o os dados descomprimidos

        int i = 0;
        int width, height;

        // Decode run_length encoded image
        unsigned char *decoded = RLE_decode(data, &width, &height); // decodifica��o de carreiras (RLE), onde tamb�m est� codificado a altura e largura da imagem.

        unsigned char *blocks = unvectorize(decoded, width*height*3); // obten��o dos blocos

        unsigned char *idctq = idct(blocks, width*height*3); // DCT inversa nos blocos

        BMPData bitmap;
        BMPData_from_raw(&bitmap, idctq, width, height); // construir a imagem a partir dos blocos gerando um cabe�alho bmp com a altura e largura da imagem
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
