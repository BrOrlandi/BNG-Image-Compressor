#include <stdlib.h>
#include <stdio.h>
#include "bitmap.h"

#ifndef RUNLEN_H

#define RUNLEN_H
#define RUN_LEN_FLAG 0

void sanitize(unsigned char *vector, int data_size);

unsigned char *encode(unsigned char *vector, int width, int height, int *size);

unsigned char *decode(unsigned char *rle, int data_size);

#endif // RUNLEN_H

