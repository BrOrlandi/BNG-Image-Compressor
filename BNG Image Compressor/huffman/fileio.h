#ifndef FILEIO_H
#define FILEIO_H

int fileWrite(char name[], unsigned char *data, unsigned int size);
unsigned char *fileRead(char name[], unsigned int *size); // return the data and writes the size

#endif // FILEIO_H
