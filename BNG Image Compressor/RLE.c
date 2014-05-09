#include<stdio.h>

#define L 16
#define C 24

int main()

{
    int output_buffer[L*C];

    int d,i,j,k;
    //FAZER ALOCAÇÃO DINAMICA DO VETOR RLE
    int rle[L*C];
    for(i=0; i<(L*C*2); i++)
        rle[i] = 0;
    int my_index[L][C];

    //PREENCHIMENTO DA MATRIZ PARA TESTE
    for (i=0; i<L; i++)
        for (j=0; j<C; j++)
        {
            my_index[i][j] = i+j+(rand()%2)+1;
        }

    //IMPRESSÃO DA MATRIZ ORIGINAL
    for(i=0; i<L; i++)
    {
        for(j=0; j<C; j++)
        {
            printf("%3d",my_index[i][j]);
        }
        printf("\n");
    }

    //ROTINA PARA VETORIZAÇÃO
    for (d=1, i=0, j=0, k=0; k<(L*C); k++)
    {
        output_buffer[k]  = my_index[i][j];
        i += d; // next index positions
        j -= d;
        if (j < 0)   // outside top
        {
            j = 0;
            d = -d; // change direction
        }
        else if (i < 0)
        {
            if (j > (C-1))   // outside left and bottom
            {
                j = (C-1);
                i += 2;
            }
            else  	// outside left
            {
                i = 0;
            }
            d = -d;
        }
        else if (i > (L-1))     // outside right
        {
            i = (L-1);
            j += 2;
            d = -d;
        }
        else if (j > (C-1))     // outside bottom
        {
            j = (C-1);
            i += 2;
            d = -d;
        }
    }

    //IMPRESSÃO DO VETOR
    printf("\n\n\n");
    for( k=0; k<(L*C); k++)
        printf("%d ",output_buffer[k]);


    printf("\n\n\n");


    //CODIFICAÇÃO POR CARREIRA
    int cont=1; //CONTA A QUANTIDADE DE REPETIÇÕES
    int flag = 0; //UTILIZAREMOS FLAG COM O VALOR ZERO, E VALORES ZERO NA MATRIZ ORIGINAL DEVEM SER SUBSTITUIDOS POR 1
    int writer = 0; //CONTA A POSIÇÃO DE ESCRITA NO VETOR CODIFICADO
    for(i=0; i<(L*C); i++)
    {
        if(i==((L*C)-1))
        {
            if(cont <= 3)
            {
                for(j=0; j<cont; j++)
                {
                    rle[writer] = output_buffer[i];
                    writer++;
                }
                cont=1;
            }
            if (cont > 3)
            {
                rle[writer] = flag;
                writer++;
                rle[writer] = cont;
                writer++;
                rle[writer] = output_buffer[i];
                writer++;
                cont=1;
            }
        }
        else
        {
            if((output_buffer[i] != output_buffer[i+1]) && (cont <= 3))
            {
                for(j=0; j<cont; j++)
                {
                    rle[writer] = output_buffer[i];
                    writer++;
                }
                cont=1;
            }
            if((output_buffer[i] != output_buffer[i+1]) && (cont > 3))
            {
                rle[writer] = flag;
                writer++;
                rle[writer] = cont;
                writer++;
                rle[writer] = output_buffer[i];
                writer++;
                cont=1;
            }
            if(output_buffer[i] == output_buffer[i+1])
            {
                cont++;
            }
        }
    }

    //IMPRESSÃO DO VETOR CODIFICADO
    printf("\n\n\n");
    for( k=0; k<writer; k++)
        printf("%d ",rle[k]);

}
