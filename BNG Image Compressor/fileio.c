#include <stdio.h.>
#include <stdlib.h.>
#include "fileio.h"
#include "bitreader.h"

unsigned char *fileRead(char name[], unsigned int *size){
    FILE *f = fopen(name, "rb");
    if(f == NULL){
        printf("File not found: %s\n",name);
        exit(1);
    }

    fseek(f,0L,SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char)*(*size));
    if(data == NULL){
        printf("Memory Error: %s\n",name);
        exit(1);
    }

    unsigned int readed = fread(data,1,*size,f);
    if(readed != *size){
        printf("Reading Error: %s\n",name);
        exit(1);
    }
    fclose(f);
    return data;
}

int huffmanFileRead(char name[], unsigned char **data, unsigned int *size){

    BitReader reader;
    BitReader_init(&reader,name);
    printf("Decompressing file %s ...\n",name);

    // Header read
    int n_bytes; // stores how many bytes are coded
    // the following arrays has the same function of the compression, but now those data will be read from file
    unsigned char bytes[256];
    unsigned int frequencies[256];

    FILE *f = reader.file;

    fread(&n_bytes,(sizeof(int)),1,f);

    int i;
    for(i=0;i<n_bytes;i++){
        fread(&(bytes[i]),sizeof(unsigned char),1,f); // read the byte
        fread(&(frequencies[i]),sizeof(unsigned int),1,f); // read his frequency

        //printf("byte = %c    freq = %d\n",bytes[i],frequencies[i]);
    }

    // Reconstructing Huffman Tree with the data read from header

    printf("Reconstructing Huffman tree.\n");
    Node *tree; // huffman tre
    tree = huffman(bytes,frequencies,n_bytes);
    Node *iterator = tree; // node to search codes in tree

    unsigned int maxSize = 100;
    (*data) = (unsigned char *)malloc(sizeof(unsigned char)*maxSize);
    unsigned int pos = 0;

    unsigned char read;
    while(BitReader_read_bit(&reader,&read)){
        if(read == 0){
            iterator = iterator->l;
        }else{
            iterator = iterator->r;
        }
        if(iterator->isLeaf == TRUE){
            //printf("%c",iterator->byte);

            // store in data
            (*data)[pos] = iterator->byte;
            pos++;
            if(pos == maxSize){ // if reach max capacity realloc
                maxSize += 100;
                (*data) = (unsigned char *)realloc((*data),maxSize*sizeof(unsigned char));
            }

            iterator = tree;
        }
    }
    *size = pos;

    BitReader_close(&reader);
    return 0;
}

int fileWrite(char name[], unsigned char *data, unsigned int size)
{
    FILE *f = fopen(name,"wb");
    fwrite(data,sizeof(unsigned char),size,f);
    fclose(f);
}

int huffmanFileWrite(char name[], unsigned char *data, unsigned int size){

    printf("Compressing file %s ...\n",name);

    unsigned char bytes[256]; // array to store the bytes reads from the file
    unsigned int frequencies[256]; // array to store the frequency of the bytes that appear in the file
    int n_bytes = 0; // will be used to count the bytes inserted in the arrays above

    unsigned int *tmpfreq = (unsigned int *)calloc(sizeof(unsigned int),256); // temporary to count bytes frequencies in file data
    printf("Counting bytes frequencies.\n");
    int i;
    for(i=0;i<size;i++){
        tmpfreq[data[i]]++; // counting number of times each byte appears on the file
    }

    for(i=0;i<256;i++){ // store in the arrays all the bytes with frequency greater than zero and their frequency
        if(tmpfreq[i] > 0){
            bytes[n_bytes] = (unsigned char)i;
            frequencies[n_bytes] = tmpfreq[i];
            n_bytes++;
        }
    }

    //unsigned char bytes[] = {1, 2, 3, 4, 5, 6};
    //unsigned int frequencies[] = {45, 13, 12, 16, 9, 5};

    Node *tree; // huffman tre

    //int size = sizeof(bytes)/sizeof(bytes[0]);

    printf("Creating Huffman tree.\n");
    tree = huffman(bytes,frequencies,n_bytes);

    printf("Generating Huffman codes.\n");
    unsigned char bytesCodes[256];
    unsigned char **codes = huffmanCodes(tree,n_bytes,bytesCodes);

// uncomment this to see bytes and their codes;
/*
    int j;
    for(i=0;i<n_bytes;i++){
        printf("Byte: %c -> ",bytesCodes[i]);
        for(j=0; codes[i][j] != 2;j++){
            printf("%d",codes[i][j]);
        }
        printf("\n");
    }
//*/
    unsigned int nSize;

    char fileNewName[strlen(name) + 4];
    strcpy(fileNewName,name);
    strcat(fileNewName,".bng");

    printf("Compressing file in: %s\n",fileNewName);

/*        for(i=0;i<n_bytes;i++){
        printf("char %c  freq %u\n",bytes[i],frequencies[i]);
    }
    printf("\n");
//*/
    huffmanCompressData(data,size,bytesCodes, bytes, frequencies,codes,n_bytes,fileNewName);

    printf("File compressed!\n");

    return 0;
}
