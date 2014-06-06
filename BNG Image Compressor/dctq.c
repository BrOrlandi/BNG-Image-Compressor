#include "dctq.h"

double cons(int a)//constantes usadas na DCT
{
    if(a==0)
        return(1/sqrt(2));
    else
        return(1);
}

unsigned char *dct(BMPData *bmp)
{
    unsigned char *blocks = bmp->block_data;
    unsigned char *dctq = calloc(bmp->dataSize, sizeof(unsigned char));

    int block_length = bmp->block_data_length;
    int k, x, y, color;


    int my_index[8][8];
    int a, b, i, j;
    int tabela[8][8] = {{16,11,10,16,24,40,51,61},//tabela do padrão JPEG
        {12,12,14,19,26,58,60,55},
        {14,13,16,24,40,57,69,56},
        {14,17,22,29,51,87,80,62},
        {18,22,37,56,68,109,103,77},
        {24,35,55,64,81,104,113,92},
        {49,64,78,87,103,121,120,101},
        {72,92,95,98,112,100,103,99}
    };
    int quantizada[8][8];
    double multi;
    double dct[8][8];

    //percorre todos os blocos
    for(k = 0; k < block_length; k++)
    {
        //percorre as 3 cores (R, G, B)
        for(color = 0; color < 3; color++)
        {
            //preenche uma matriz 8x8 de inteiros para os cálculos
            for(a=0; a<8; a++)
            {
                for(b=0; b<8; b++)
                {
                    my_index[a][b] = blocks[block_offset(k,a,b,color)];
                    //printf("%d ", my_index[a][b]);
                }
                //printf("\n");
            }
            //printf("\n");

            //DCT
            for(i=0; i<8; i++)
            {
                for(j=0; j<8; j++)
                {
                    //cálculo do valor de cada coordenada da matriz transformada
                    multi = 0;
                    for(x=0; x<8; x++)
                    {
                        for(y=0; y<8; y++)
                        {
                            multi = multi + (my_index[x][y]*cos(((2*x+1)*i*M_PI)/16)*cos(((2*y+1)*j*M_PI)/16));
                        }
                    }
                    dct[i][j] = 0.25*cons(i)*cons(j)*multi;
                }
            }
            //Quantização
            for(i=0; i<8; i++)
            {
                for(j=0; j<8; j++)
                {
                    quantizada[i][j] = (int)round(dct[i][j]/tabela[i][j]);
                    dctq[block_offset(k, i, j, color)] = quantizada[i][j]+128;
                    //valor 128 subtraído será somado na descompressão, necessário para não estourar o tipo de dados uchar
                    //printf("%d ", dctq[block_offset(k, i, j, color)]);
                }
                //printf("\n");
            }
            //printf("\n");
        }
    }
    return dctq;
}

unsigned char *idct(unsigned char *blocks, int size)
{
    unsigned char *idctq = calloc(size, sizeof(unsigned char));

    int block_length = size / (8 * 8 * 3);
    int k, x, y, color;

    int tabela[8][8] = {{16,11,10,16,24,40,51,61},//tabela padrão JPEG
        {12,12,14,19,26,58,60,55},
        {14,13,16,24,40,57,69,56},
        {14,17,22,29,51,87,80,62},
        {18,22,37,56,68,109,103,77},
        {24,35,55,64,81,104,113,92},
        {49,64,78,87,103,121,120,101},
        {72,92,95,98,112,100,103,99}
    };

    int quantizada[8][8];
    int desquantizada[8][8];
    double multi;
    int a,b,i,j;
    int idctt[8][8];

    //percorre cada bloco
    for(k = 0; k < block_length; k++)
    {
        //percorre cada cor
        for(color = 0; color < 3; color++)
        {
            //preenche matriz de int e soma de volta vlr 128
            for(a=0; a<8; a++)
            {
                for(b=0; b<8; b++)
                {
                    if(blocks[block_offset(k, a, b, color)] == 1){
                        quantizada[a][b] = 0;}
                    else{
                    quantizada[a][b] = blocks[block_offset(k, a, b, color)]-128;
                    //printf("%d ", quantizada[a][b]);
                    }
                }
                //printf("\n");
            }
            //printf("\n");

            //DesQuantização
            for(i=0; i<8; i++)
            {
                for(j=0; j<8; j++)
                {
                    desquantizada[i][j] = quantizada[i][j]*tabela[i][j];
                }
            }
            //IDCT - DCT inversa
            for(i=0; i<8; i++)
            {
                for(j=0; j<8; j++)
                {
                    multi = 0;
                    for(x=0; x<8; x++)
                    {
                        for(y=0; y<8; y++)
                        {
                            multi = multi + (cons(x)*cons(y)*desquantizada[x][y]*cos(((2*i+1)*x*M_PI)/16)*cos(((2*j+1)*y*M_PI)/16));
                        }
                    }
                    idctt[i][j] = (int)round(0.25*multi);
                    idctq[block_offset(k, i, j, color)] = idctt[i][j];
                    //printf("%d ", idctq[block_offset(k, i, j, color)]);
                }
                //printf("\n");
            }
            //printf("\n");
        }
    }
    return idctq;
}
