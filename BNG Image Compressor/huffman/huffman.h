/**
*
*  The Huffman Compressed File Description.
*
*  The compressed file contains the following data:
*  [Huffman Header] [Data compressed] [Last byte]
*
*  The Huffman Header contains pairs of Characters and their Frequencies to reconstruct the Huffman Tree.
*  [Number of pairs] {[character][frequency] ... }
*
*  The Last Byte is used to describe how much bits in the last byte in data are valid.
*
*/

#ifndef HUFFMAN_H
#define HUFFMAN_H

#define MAX_CODE_LENGTH 50

#include "tree.h"
#include "bitwriter.h"
#include "bitreader.h"
#include <stdlib.h>


typedef struct{
    unsigned int frequencies[256];
    unsigned int bytes_count;  // count number of bytes to generate code, up to 256

    unsigned char *codes[256]; // array of strings, each string is a binary code of the byte

    Node *htree;

    // uc : uncompressed
    unsigned char **uc_data; // blocks of data to compress
    unsigned int uc_blocks;
    unsigned int *uc_sizes;

} Huffman;

/**
    Initializes the Huffman data structure
*/
void Huffman_init(Huffman *h);

/**
    Add a data block to the structure, all data blocks will be compressed.
*/
void Huffman_add_data_block(Huffman *h, unsigned char *data,unsigned int size);


/**
    Count frequencies of bytes in data blocks
*/
void Huffman_count_frequencies(Huffman *h);

/**
    Build Huffman Tree using frequencies of bytes
*/
void Huffman_build_tree(Huffman *h);

/**
    Generate the Hufffman Codes for each byte
*/
void Huffman_generate_codes(Huffman *h);

/**
    Apply the 3 last functions above, so the process to compress is Init, Add Data Block, Apply, Compress Data to File.
*/
void Huffman_apply(Huffman *h);

/**
    Will compress all the data using the generated Huffman codes.
*/
void Huffman_compress_data_to_file(Huffman *h, char filename[]);

/**
    Free the memory used by the structure
*/
void Huffman_free(Huffman *h);

/**
    Uncompress a file compressed by this algorithm, will put one data block wich is the data uncompressed from file.
*/
void Huffman_file_decompress(Huffman *h, char filename[]);


#endif // HUFFMAN_H

