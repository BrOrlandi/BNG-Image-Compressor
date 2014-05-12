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
    Pega todos os valores que s�o iguais a flag e incrementa.
*/
void RLE_sanitize(unsigned char *vector, unsigned int data_size);

/**
    Codifica o vetor por carreiras, ir� retornar o vetor codificado e o seu tamanho em size.
*/
unsigned char *RLE_encode(unsigned char *vector, unsigned int width, unsigned int height, unsigned int *size);

/**
    Decodifica o vetor por carreiras
*/
unsigned char *RLE_decode(unsigned char *rle, unsigned int *width, unsigned int *height);

#endif // RUNLEN_H

