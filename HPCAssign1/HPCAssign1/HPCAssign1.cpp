#define _CRT_SECURE_NO_DEPRECATE
#include <mpi.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int rank, size;             // Rank and Size of processors
    int col, row;               // column and row of matrix
    float* matrix = NULL;       // The float matrix (Need to "malloc")
    int i, j;                   // loop counter
    int n;                      // Iterator

    FILE* fpin = NULL;                              // pointer to input file
    FILE* fpout = NULL;                             // pointer to output file
    char inputFileName[30], outputFileName[30];     // Name of input and output file

    double startTime, endTime;  // Timer variable

    /*____________ Check argument ____________*/
    if (argc == 4)
    {
        strcpy(inputFileName, argv[1]);
        strcpy(outputFileName, argv[2]);
        n = atoi(argv[3]);
    }
    else
    {
        return 0;
    }

    MPI_Init(&argc, &argv);                 // initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // Rank of processor
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Amount of processor

    if (rank == 0)
    {
        fpin = fopen(inputFileName, "r");
        if (!fpin)
            return 1;                       // return 1 if if file not found.

        if (fscanf(fpin, "%d %d", &row, &col))
            printf("row: %d  col: %d\n\n", row, col);
        else
        {
            printf("Can't read file.\n");
            return 1;
        }

        /*___________ Allocate Matrix ___________*/
        matrix = (float*)malloc(row * col * sizeof(float));

        /*___________ Read matrix from file ___________*/
        while (!feof(fpin))
        {
            for (i = 0; i < row; i++)   // i as number of row
            {
                for (j = 0; j < col; j++)   // j as number of column 
                {
                    fscanf(fpin, "%f", &matrix[(i * col) + j]);     // store as 1D array
                }
            }
        }
        fclose(fpin);
    } // End read file in rank 0

    startTime = MPI_Wtime();

    // TO DO: calculate heat transfer for n times

    endTime = MPI_Wtime();
    if (rank == 0)
    {
        /*__________ Write file ___________*/
        fpout = fopen(outputFileName, "w+");
        if (!fpout)
            return 1;
        fprintf(fpout, "%d %d\n", row, col);        // Write row and column
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                fprintf(fpout, "%.0f ", matrix[(i * col) + j]);
            }
            fprintf(fpout, "\n");
        }
        free(matrix);
        fclose(fpout);
    }

    printf("%lf", endTime - startTime);
    MPI_Finalize();
    return 0;
}

