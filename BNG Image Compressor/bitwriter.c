#include "bitwriter.h"

int BitWriter_init(BitWriter *writer, char *filename) {
    writer->buffer = 0;
    writer->bit_number = 0;

    writer->file = fopen(filename, "wb");

    return (int) writer->file;
}

int BitWriter_write_bit(BitWriter *writer, unsigned char bit) {
    // Prepare the bit so it has one set signal (won't corrupt anything)
    bit &= 1;
    // Shift buffer one position
    writer->buffer <<= 1;
    // Set the value on the proper position
    writer->buffer |= bit;

    writer->bit_number++;
    // In case of buffer being full, we flush everything to the file
    if(writer->bit_number == 8) {
        return BitWriter_flush(writer);
    }

    return 0;
}

int BitWriter_write_bits(BitWriter *writer, unsigned char bitstream, int count) {
    int i;
    int retval = 0;
    for(i = 0; i < count; i++) {
        // Get the left most bit and walks to the right untill the end
        unsigned char bit = (bitstream & (1 << (count - i - 1)))?1:0;
        retval |= BitWriter_write_bit(writer, bit);
    }
    return retval;
}

int BitWriter_flush(BitWriter *writer) {
    // If there are no contents, there's nothing to write
    if(writer->bit_number == 0) {
        return 0;
    }

    writer->buffer <<= 8 - writer->bit_number;

    int retval = fwrite(&(writer->buffer), sizeof(char), 1, writer->file);
    writer->buffer = 0;
    writer->bit_number = 0;

    return !retval;
}

int BitWriter_close(BitWriter *writer) {
    int flush = BitWriter_flush(writer);
    int close = fclose(writer->file);
    return flush || close;
}
