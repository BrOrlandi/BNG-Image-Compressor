#include <stdlib.h>
#include <stdio.h>
#include "bitmap.h"

#ifndef RUNLEN_H

#define RUNLEN_H
/**
    Esta flag sera usada para indicar que o proximo valor corresponde a quantas vezes o valor repete no vetor
*/
#define RUN_LEN_FLAG 0

/**
    Pega todos os valores que são iguais a flag e incrementa.
*/
void RLE_sanitize(unsigned char *vector, int data_size);

/**
    Codifica o vetor por carreiras, irá retornar o vetor codificado e o seu tamanho em size.
*/
unsigned char *RLE_encode(unsigned char *vector, int width, int height, int *size);

/**
    Decodifica o vetor por carreiras
*/
unsigned char *RLE_decode(unsigned char *rle, int data_size);

#endif // RUNLEN_H

