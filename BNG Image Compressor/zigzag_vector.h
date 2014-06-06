#include <stdlib.h>
#include <stdio.h>
#include "bitmap.h"

#ifndef ZIGZAG_H

#define ZIGZAG_H

unsigned char *vectorize(BMPData *bmp, unsigned char *dctq);

unsigned char *unvectorize(unsigned char *zigzag, int size);

#endif // ZIGZAG_H
