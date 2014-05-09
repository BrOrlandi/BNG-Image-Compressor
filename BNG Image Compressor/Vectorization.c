#include<stdio.h>

#define L 16
#define C 24

int main()

{
    int output_buffer[L*C];

    int d=1,i=0,j=0,k=0;
    int new_matrix[L][C];

    int my_index[L][C];

    for (i=0; i<L; i++)
        for (j=0; j<C; j++)
        {
            my_index[i][j] = i+j+(rand()%5);
        }

    for(i=0; i<L; i++)
    {
        for(j=0; j<C; j++)
        {
            printf("%3d",my_index[i][j]);
        }
        printf("\n");
    }

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

    printf("\n\n\n");
    for( k=0; k<(L*C); k++)
        printf("%d ",output_buffer[k]);


    printf("\n\n\n");



    /* ---reconstruction of the matrix  from the output buffer -----------------*/

    // this part(anti-zigzag ) is not  reconstructing the matrix.


    for ( d=1,i=0,j=0,k=0; k<(L*C); k++)

    {
        new_matrix[i][j] = output_buffer[k];
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



    for(i=0; i<L; i++)

    {
        for(j=0; j<C; j++)

        {
            printf("%3d",new_matrix[i][j]);

        }

        printf("\n");

    }




}
