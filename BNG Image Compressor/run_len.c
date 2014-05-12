#include "run_len.h"

void RLE_sanitize(unsigned char *vector, int data_size) {
    int i;
    for(i = 0; i < data_size; i++) {
        if(vector[i] == RUN_LEN_FLAG) {
            vector[i]++;
        }
    }
}

unsigned char *RLE_encode(unsigned char *vector, int width, int height, int *size) {
    int data_size = width * height * 3;
    unsigned char *rle = (unsigned char*) calloc(data_size+2+8, sizeof(unsigned char));

    RLE_sanitize(vector, data_size);

    //CODIFICAÇÃO POR CARREIRA
    int count = 1; //CONTA A QUANTIDADE DE REPETIÇÕES
    int writer = 0; //CONTA A POSIÇÃO DE ESCRITA NO VETOR CODIFICADO
    int i, j;

    // These are reserved fields, they store the matrix original width
    // and height
    rle[writer++] = (width & 0xFF);
    rle[writer++] = (width >> 8) & 0xFF;;
    rle[writer++] = (width >> 16) & 0xFF;
    rle[writer++] = (width >> 24) & 0xFF;

    rle[writer++] = (height & 0xFF);
    rle[writer++] = (height >> 8) & 0xFF;
    rle[writer++] = (height >> 16) & 0xFF;
    rle[writer++] = (height >> 24) & 0xFF;

    unsigned char last_char = vector[0];
    for(i = 1; i<data_size; i++)
    {
        if(last_char == vector[i]) { // Keep counting the appearances
            count++;
            if(count >= 255) {
                goto FLUSH;
            }
        }
        else {
            if(count >= 3) { // If count is greater than 3, value is run-length coded
                FLUSH:
                rle[writer++] = RUN_LEN_FLAG;
                rle[writer++] = count;
                rle[writer++] = last_char;
                count = 1;
            }
            else {
                for(j = 0; j < count; j++) {
                    rle[writer++] = last_char;
                }
                count = 1;
            }
        }
        last_char = vector[i];
    }
    if(count >= 3) { // If count is greater than 3, value is run-length coded
        rle[writer++] = RUN_LEN_FLAG;
        rle[writer++] = count;
        rle[writer++] = last_char;
        count = 1;
    }
    else {
        for(j = 0; j < count; j++) {
            rle[writer++] = last_char;
        }
        count = 1;
    }
    // Duas flags são o valor sentinela que representa o fim da decodificação.
    rle[writer++] = RUN_LEN_FLAG;
    rle[writer++] = RUN_LEN_FLAG;
//    for(j = 0; j < writer; j++) {
//        printf("%d ",rle[j]);
//    }
//    printf("\n");
//    for(j = 0; j < data_size; j++) {
//        printf("%d ",vector[j]);
//    }
    printf("\n");

    *size = writer;
    return rle;
}

unsigned char *RLE_decode(unsigned char *rle, int *width, int *height) {

    // Recuperando os dados que indicam a altura e largura da imagem
    // Estes dados fora codificados
    *width = rle[0];
    *width |= (rle[1] & 255) << 8;
    *width |= (rle[2] & 255) << 16;
    *width |= (rle[3] & 255) << 24;

    *height = rle[4];
    *height |= (rle[5] & 255) << 8;
    *height |= (rle[6] & 255) << 8;
    *height |= (rle[7] & 255) << 8;

    int i = 8, j, count = 0;
    int data_size = (*width) * (*height) * 3;

    unsigned char *rle_dec = (unsigned char*) calloc(data_size, sizeof(unsigned char));

    while(1) {
        //printf("%d -> %d << %d\n", i, data_size, count);
        if(rle[i++] == RUN_LEN_FLAG) {
            if(rle[i++] == RUN_LEN_FLAG) {
                break;
            }
            for(j = 0; j < rle[i-1]; j++) {
                rle_dec[count++] = rle[i];
            }
            i++;
        }
        else {
            rle_dec[count++] = rle[i-1];
        }
    }

    return rle_dec;
}
