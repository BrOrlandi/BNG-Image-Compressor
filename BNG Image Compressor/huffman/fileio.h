#ifndef FILEIO_H
#define FILEIO_H

int fileWrite(FILE *file, unsigned char *data, unsigned int size);
unsigned char *fileRead(char name[], unsigned int *size); // return the data and writes the size

#endif // FILEIO_H
