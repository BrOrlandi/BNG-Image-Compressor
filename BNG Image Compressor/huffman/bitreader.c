#include "bitreader.h"

int BitReader_init(BitReader *reader, char *filename){
    reader->buffer = 0;
    reader->isAtLastByte = 0;
    reader->bit_number = 8; // forces the first load of the buffer
    reader->file = fopen(filename,"rb");
    if(reader->file == NULL){
        printf("File not found: %s\n",filename);
        exit(1);
    }

    // read the last byte
    fseek(reader->file,-1,SEEK_END);
    reader->lastBytePosition = ftell(reader->file);
    fread(&(reader->lastByteInfo),sizeof(unsigned char),1,reader->file);
    fseek(reader->file, 0, SEEK_SET);

    //printf("Last byte info: %d\n", reader->lastByteInfo);
    //printf("Last byte pos: %d\n", reader->lastBytePosition);

    return (int) reader->file;
}

int BitReader_read_bit(BitReader *reader, unsigned char *bit){
    if(reader->bit_number == 8){
        reader->bit_number = 0;
        fread(&(reader->buffer),sizeof(unsigned char),1,reader->file);
        if(ftell(reader->file) == reader->lastBytePosition){
            //printf("reaches last byte--------------\n");
            reader->isAtLastByte = 1;
        }
    }
    if(reader->isAtLastByte == 1 && reader->bit_number == (int)reader->lastByteInfo){
        return 0;
    }
    unsigned char tmp = reader->buffer;

    tmp >>= 7 - reader->bit_number;
    reader->bit_number++;
    *bit = tmp&1;

    return 1;
}

int BitReader_close(BitReader *reader){
    fclose(reader->file);
}
