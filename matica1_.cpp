#include <stdio.h>
#include <stdlib.h>

typedef struct{
unsigned int rows;
unsigned int cols;
float *elem;
}MAT;

#define ELEM(mat, i, j) (mat->elem[(i-1)* mat->cols + j - 1])

void mat_print(MAT *mat)
{
	printf("Vypis Matice: \n");
	/*	for(int i=0;i<(mat->rows*mat->cols);i++){
		printf("%f\t",mat->elem[i]);
		if((i+1) % mat->cols == 0)
			printf("\n");
	}*/
	for(int i=0;i<(mat->rows);i++)
	{
		for(int j=0;j<(mat->cols);j++)
			printf("%f\t",mat->elem[i*mat->cols + j]);
		printf("\n");

	}
}


main() 
{
	MAT maticaA;
	float hodnoty[] = {1,3,3,8,2,2,6,2,4,4,7,3,1,1,0,9};
	maticaA.rows = 4;
	maticaA.cols = 4;
	maticaA.elem = hodnoty;
	mat_print(&maticaA);
}
