/**
*
*  HFF Huffman File Format.
*
*  The file contains the following data:
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

#include "tree.h"

Node *huffman(unsigned char bytes[],unsigned int frequencies[], int size);
unsigned char ** huffmanCodes(Node *tree, int size, unsigned char bytes[]);
void huffmanCompressData(unsigned char *data, unsigned int sizeOfData, unsigned char *bytesCodes, unsigned char *bytes, unsigned int *frequencies, unsigned char **codes, unsigned int sizeOfCodes, char fileToSave[]);

int endsWith(char name[], char end[]);

#endif // HUFFMAN_H
