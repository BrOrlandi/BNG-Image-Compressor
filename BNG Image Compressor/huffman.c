#include "huffman.h"
#include "tree.h"
#include <string.h>
#include "bitwriter.h"

#define MAX_CODE_LENGTH 50
#define NULL 0


void sortNodes(Node **nodes, int size){ // an Insertion Sort implementation
    int i,j;
    Node *aux;
    Node *greatest;
    int greatestPos;
    for(i=0;i<size;i++){
        greatest = nodes[i];
        greatestPos = i;
        for(j=i+1;j<size;j++){
            if(nodes[j]->frequency > greatest->frequency){
                greatest = nodes[j];
                greatestPos = j;
            }
        }
        aux = nodes[i];
        nodes[i] = nodes[greatestPos];
        nodes[greatestPos] = aux;
    }
}

Node *huffman(unsigned char bytes[],unsigned int frequencies[], int size){

    Node **array = (Node **) malloc(sizeof(Node *)*size);

    int i;
    for(i=0;i<size;i++){
        array[i] = newNode(bytes[i],frequencies[i],TRUE);
    }

    sortNodes(array,size);
//uncomment this to see the bytes and their frequencies
/*
    for(i=0;i<size;i++){
        printf("char %c  freq %u\n",array[i]->byte,array[i]->frequency);
    }
    printf("\n");
//*/
    Node * newnode;

    while(size > 1){
        newnode = newNode(0,array[size-1]->frequency + array[size-2]->frequency, FALSE);
        insertNodeLeft(newnode,array[size-1]);
        insertNodeRight(newnode,array[size-2]);
        array[size-1] = NULL;
        array[size-2] = newnode;
        size--;

        sortNodes(array,size);

/*        for(i=0;i<size;i++){
            printf("char %d  freq %d\n",array[i]->byte,array[i]->frequency);
        }
        printf("\n"); */
    }

    return array[0];
}

// depth-first to reach the codes
void depthCodes(Node *root, char tcode[], int last, unsigned char bytes[], char **codes, int *lastCode){
    if(root->l != NULL){
        tcode[last] = 0;
        depthCodes(root->l,tcode,last+1,bytes,codes,lastCode);
    }
    if(root->r != NULL){
        tcode[last] = 1;
        depthCodes(root->r,tcode,last+1,bytes,codes,lastCode);
    }
    if(root->isLeaf == TRUE){
        int i;
        for(i=0;i<last;i++){
            codes[*lastCode][i] = tcode[i];
        }
        codes[*lastCode][last] = 2;
        bytes[*lastCode] = root->byte;
        (*lastCode)++;
    }
}

unsigned char **huffmanCodes(Node *tree, int size, unsigned char bytes[]){

    unsigned char **codes = (char **) malloc(sizeof(char *)*size);
    int i;
    for(i=0;i<size;i++){
        codes[i] = (char *)malloc(sizeof(char)*MAX_CODE_LENGTH);
    }

    int lastCode = 0;

    char tcode[MAX_CODE_LENGTH];
    depthCodes(tree,tcode,0,bytes,codes,&lastCode);

    return codes;
}

int endsWith(char name[], char end[]){
    int len = strlen(name);
    int elen = strlen(end);
    if(len < elen)
        return 0;
    return strncmp(name + len - elen, end, elen) == 0;
}

void huffmanCompressData(unsigned char *data, unsigned int sizeOfData, unsigned char *bytesCodes, unsigned char *bytes, unsigned int *frequencies, unsigned char **codes, unsigned int sizeOfCodes, char fileToSave[]){
    unsigned char *byteToCode[256];
    unsigned int i,j;

    BitWriter writer;
    BitWriter_init(&writer,fileToSave); // write data bits in the compressed file

    // start writing the header of this Huffman compression

    fwrite(&sizeOfCodes,sizeof(int),1,writer.file); // write the number of bytes that have a code
    //printf("sizeOfCodes = %d\n",sizeOfCodes);
    unsigned char *code;
    for(i=0;i<sizeOfCodes;i++){
        byteToCode[(int)bytesCodes[i]] = codes[i]; // makes an easy array to find the code of byte like a hash
        fwrite(&(bytes[i]),sizeof(unsigned char),1,writer.file); // write the byte
        fwrite(&(frequencies[i]),sizeof(unsigned int),1,writer.file); // write his frequency
/*
        printf("byte = %c    freq = %d\n",bytes[i],frequencies[i]);

        printf("%c = ",bytesCodes[i]);
        code = codes[i]; // each code is translated here
        for(j=0; code[j] != 2;j++){
            printf("%d",code[j]);
        }
        printf("\n");
//        */
    }

    //int count = 0;
    for(i=0;i<sizeOfData;i++){
        code = byteToCode[data[i]]; // each code is translated here

        for(j=0; code[j] != 2;j++){
            BitWriter_write_bit(&writer,code[j]); // write the each bit of the code
            //count++;
            //printf("%d",code[j]);
            //if(count%8==0)
            //    printf("\n");
        }
    }
    // the last byte may have zeros on the right, so a new byte is written at the end to just tell how many bits are valid in the last byte of data compressed.
    char lastByte = (char)writer.bit_number;
    BitWriter_flush(&writer);
    BitWriter_write_bits(&writer,lastByte,8);

    BitWriter_close(&writer);
}
