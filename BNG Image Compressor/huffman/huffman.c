#include "huffman.h"

#define MAX_ALLOCATION 100000

void Huffman_init(Huffman *h){
    int i;
    for(i=0;i<256;i++){
        h->frequencies[i] = 0;
        h->codes[i] = NULL;
    }
    h->bytes_count = 0;
    h->uc_data = NULL;
    h->uc_sizes = NULL;
    h->uc_blocks = 0;
}


void Huffman_add_data_block(Huffman *h, unsigned char *data,unsigned int size){
    h->uc_blocks += 1;
    h->uc_data = (unsigned char **)realloc(h->uc_data, sizeof(unsigned char *)*h->uc_blocks);
    h->uc_sizes = (unsigned int *)realloc(h->uc_sizes, sizeof(unsigned int)*h->uc_blocks);
    h->uc_data[h->uc_blocks-1] = data;
    h->uc_sizes[h->uc_blocks-1] = size;
}

void Huffman_count_frequencies(Huffman *h){
    int i,j;
    for(i=0;i<h->uc_blocks;i++){
        for(j=0;j<h->uc_sizes[i];j++){
            h->frequencies[h->uc_data[i][j]]++;
        }
    }
    for(i=0;i<256;i++){
        if(h->frequencies[i] > 0){
            h->bytes_count++;
        }
    }
}

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

void Huffman_build_tree(Huffman *h){

    Node **array = (Node **) malloc(sizeof(Node *)*h->bytes_count);

    int i,count=0;
    for(i=0;i<256;i++){
        if(h->frequencies[i] > 0){
            array[count] = Tree_newNode(i,h->frequencies[i],TRUE);
            count++;
        }
    }

    sortNodes(array,h->bytes_count);
//uncomment this to see the bytes and their frequencies
/*
    for(i=0;i<h->bytes_count;i++){
        printf("char %d  freq %u\n",array[i]->byte,array[i]->frequency);
    }
    printf("\n");
//*/

    Node * newnode;

    while(count > 1){
        newnode = Tree_newNode(0,array[count-1]->frequency + array[count-2]->frequency, FALSE);
        Tree_insertNodeLeft(newnode,array[count-1]);
        Tree_insertNodeRight(newnode,array[count-2]);
        array[count-1] = NULL;
        array[count-2] = newnode;
        count--;

        sortNodes(array,count);

    }

    h->htree = array[0];

    free(array);
}

// depth-first to reach the codes
void depthCodes(Node *root, char tcode[], int last, unsigned char **codes){
    if(root->l != NULL){
        tcode[last] = 0;
        depthCodes(root->l,tcode,last+1,codes);
    }
    if(root->r != NULL){
        tcode[last] = 1;
        depthCodes(root->r,tcode,last+1,codes);
    }
    if(root->isLeaf == TRUE){
        int i;
        for(i=0;i<last;i++){
            codes[root->byte][i] = tcode[i];
        }
        codes[root->byte][last] = 2;
    }
}

void Huffman_generate_codes(Huffman *h){

    int i;
    for(i=0;i<256;i++){
        if(h->frequencies[i] > 0)
            h->codes[i] = (unsigned char *)malloc(sizeof(unsigned char)*MAX_CODE_LENGTH);
    }
    char tcode[MAX_CODE_LENGTH];
    depthCodes(h->htree,tcode,0,h->codes);

}

void Huffman_apply(Huffman *h){
    Huffman_count_frequencies(h);
    Huffman_build_tree(h);
    Huffman_generate_codes(h);
}

void Huffman_compress_data_to_file(Huffman *h, char filename[]){
    unsigned int i,j;

    BitWriter writer;
    BitWriter_init(&writer,filename); // write data bits in the compressed file

    // start writing the header of this Huffman compression

    fwrite(&h->bytes_count,sizeof(int),1,writer.file); // write the number of bytes that have a code
    //printf("h->bytes_count = %d\n",h->bytes_count);
    unsigned char *code;

    int c;
    for(c=0;c<256;c++){
        if(h->frequencies[c]>0){
            unsigned char c2 = (unsigned char)c;
            fwrite(&(c2),sizeof(unsigned char),1,writer.file); // write the byte
            fwrite(&(h->frequencies[c]),sizeof(unsigned int),1,writer.file); // write his frequency
            //printf("writing: %c, freq: %d\n",c,h->frequencies[c]);
        }
    }

    //int count = 0;
    int b;
    for(b=0;b<h->uc_blocks;b++){
        for(i=0;i<h->uc_sizes[b];i++){
            code = h->codes[h->uc_data[b][i]]; // each code is translated here

            for(j=0; code[j] != 2;j++){
                BitWriter_write_bit(&writer,code[j]); // write the each bit of the code
            }
        }
    }
    // the last byte may have zeros on the right, so a new byte is written at the end to just tell how many bits are valid in the last byte of data compressed.
    char lastByte = (char)writer.bit_number;
    BitWriter_flush(&writer);
    BitWriter_write_bits(&writer,lastByte,8);

    BitWriter_close(&writer);
}

void Huffman_free(Huffman *h){
    int c,b;
    for(c=0;c<256;c++){
        if(h->codes[c] != NULL){
            free(h->codes[c]);
        }
    }
    Tree_free(h->htree);

    for(b=0;b<h->uc_blocks;b++){
        free(h->uc_data[b]);
    }
    free(h->uc_data);
    return;
}

void Huffman_file_decompress(Huffman *h, char filename[]){
    BitReader reader;
    BitReader_init(&reader,filename);

    // Header read
    unsigned char byte;

    FILE *f = reader.file;

    fread(&h->bytes_count,(sizeof(int)),1,f);

    int i;
    for(i=0;i<h->bytes_count;i++){
        fread(&(byte),sizeof(unsigned char),1,f); // read the byte
        fread(&(h->frequencies[byte]),sizeof(unsigned int),1,f); // read his frequency

        //printf("byte = %c    freq = %d\n",byte,h->frequencies[byte]);
    }

    // Reconstructing Huffman Tree with the data read from header
    Huffman_build_tree(h);
    Node *iterator = h->htree; // node to search codes in tree

    unsigned int maxSize = MAX_ALLOCATION;
    unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char)*maxSize);
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
            data[pos] = iterator->byte;
            pos++;
            if(pos == maxSize){ // if reach max capacity realloc
                maxSize += MAX_ALLOCATION;
                data = (unsigned char *)realloc(data,maxSize*sizeof(unsigned char));
            }

            iterator = h->htree;
        }
    }
    unsigned int size = pos;

    BitReader_close(&reader);

    Huffman_add_data_block(h,data,size);

    return ;
}
