#ifndef BITREADER_H
#define BITREADER_H

#include <stdlib.h>
#include <stdio.h>

/**
    This BitReader must be used to read compressed data.
    The last byte of data may have right zeros that are not significant
    So the reader will read the last byte of file to indicate how many bits the last byte of data.
*/

typedef struct BitReader {

    // Byte read from the file
    unsigned char buffer;
    // Number of the next bit to be read in the buffer.
    int bit_number;
    // File pointer for reader
    FILE *file;

    unsigned int lastBytePosition; // store the position of the last byte, calculated at BitReader_init
    char lastByteInfo; // last byte

    char isAtLastByte; // to check if is reading the last byte

} BitReader;


/**
    Initializes a Bitreader, all bitreader files are opened in "rb" mode.
 */
int BitReader_init(BitReader *reader, char *filename);

/**
    Read a bit from file, returning 1 if success or 0 if reach the end of file
 */
int BitReader_read_bit(BitReader *reader, unsigned char *bit);

/**
    Close the reader
 */
int BitReader_close(BitReader *reader);

#endif // BITREADER_H
