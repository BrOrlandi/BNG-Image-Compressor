#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bitmap.h"
// multithread no Windows:
#include <process.h>
#include <windows.h>

#ifndef DCT_H

#define DCT_H


unsigned char *dct(BMPData *bmp);

unsigned char *idct(unsigned char *blocks, int size);

#endif // DCT_H
