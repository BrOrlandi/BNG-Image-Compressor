#include <stdlib.h>
#include <stdio.h>

#ifndef BITWRITER

typedef struct BitWriter {

    // Bits to be written in the FILE
    unsigned char buffer;
    // Number of bit that where currently written in the buffer
    int bit_number;
    // File pointer for writting
    FILE *file

} BitWriter;

/**
    Initializes a bitwriter, all bitwriter files are opened in "wb" mode.
 */
int BitWriter_init(BitWriter *writer, char *filename);

/**
    Write a bit to the bitwriter, if the bitwriter buffer is full it will
    flush it to the file (flushing does not mean calling fflush to force the
    writing).
 */
int BitWriter_write_bit(BitWriter *writer, unsigned char bit);

/**
    Writes a stream of bits into the file.

    @param count The number of bit to extract from the bitstream
*/
int BitWriter_write_bits(BitWriter *writer, unsigned char bitstream, int count);

/**
    Force writing every content on the buffer to the file
 */
int BitWriter_flush(BitWriter *writer);

/**
    Flushes the buffer and closes the writer
 */
int BitWriter_close(BitWriter *writer);

#endif // BITWRITER
