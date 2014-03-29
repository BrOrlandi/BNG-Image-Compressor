#ifndef FILEIO_H
#define FILEIO_H

#include "tree.h"

unsigned char *fileRead(char name[], unsigned int *size); // return the data and writes the size
int fileWrite(char name[], unsigned char *data, unsigned int size);

int huffmanFileRead(char name[], unsigned char **data, unsigned int *size);// returns the data decompressed
int huffmanFileWrite(char name[], unsigned char *data, unsigned int size);
#endif // FILEIO_H
