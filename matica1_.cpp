#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void mat_unit(MAT *mat)
{
	printf("Spusta sa mat_unit\n");
	for(int i=0;i<(mat->rows);i++)
	{
		for(int j=0;j<(mat->cols);j++)
		{
			if(i==j){
				//printf("nasiel diagonalu %d %d", i + 1 , j + 1);
				ELEM(mat, i + 1, j + 1) = 1;
			}
			if(i!=j)
				ELEM(mat, i + 1, j + 1) = 0; 
		}	
	}
}

void mat_random(MAT *mat)
{
	printf("Spusta sa mat_random\n");
	srand(time(NULL));
	for(int i=0;i<(mat->rows);i++)
	{
		for(int j=0;j<(mat->cols);j++)
		{
			//vypocet random cisla z rozshahu -1 a 1 
			//float(rand()) / (float)(RAND_MAX)) * (MAX - MIN) + MIN
			ELEM(mat, i + 1, j + 1) = (float(rand()) / (float)(RAND_MAX)) * ( 1 - (-1)) + (-1); 
		}	
	}
}

void mat_destroy(MAT *mat){
	free(mat);
} 

float mat_trace(MAT *mat){
	float stopa=0;
	if (mat->cols != mat->rows)
		{
			printf("Matica nie je NxN");
			return 1;
		}
//	printf("Pocitame Stopu\n");
	for(int i=0;i<(mat->rows);i++)
	{
		for(int j=0;j<(mat->cols);j++)
		{
			if(i==j){
				//printf("nasiel diagonalu %d %d", i + 1 , j + 1);
				stopa += ELEM(mat, i + 1, j + 1);
			//	printf("%f\t",stopa);
			}
			
		}	
	}
	printf("Stopa matice je %f\t",stopa);
	return stopa;
}

MAT *mat_create_with_type(unsigned int rows, unsigned int cols)
{
	MAT *ptr; 
	ptr = (MAT*) malloc(sizeof(unsigned int) * 2 + rows*cols*sizeof(float));
	if(ptr == NULL)                     
    {
        printf("Error! memory not allocated.");
        free(ptr);
    }
	return ptr; 
}

MAT *mat_create_by_file(char *matrix)
{
	FILE *ptr;
	ptr = fopen(matrix,"rb");	// r for read, b for binary
	
	char mat, typ;
	unsigned int rows, cols; 
	
	fread(&mat,sizeof(mat),1,ptr);	
	fread(&typ,sizeof(typ),1,ptr); 
	fread(&rows,sizeof(rows),1,ptr);
	fread(&cols,sizeof(cols),1,ptr);

	MAT *matica;
	matica = mat_create_with_type(rows,cols);
	
	printf("Nacitali sme schemu: %c %c %u %u\n", mat, typ, rows, cols);
	/*printf("Nacitali sme elementy:\n");
	for (int i=0;i < (matica->rows * matica->cols);i++) 
	{
    	printf("%lf\n",matica->elem[i]);
	}*/
	printf("Element 2 2: %f\n", ELEM(matica, 1, 1));
	fflush(stdin);
	fread(matica->elem,sizeof(matica->elem),1,ptr);
	

}

main() 
{
	MAT maticaA;
	MAT *maticaB;
	float hodnoty[] = {1,3,3,8,2,2,6,2,4,4,7,3,1,1,0,9};
	maticaA.rows = 4;
	maticaA.cols = 4;
	maticaA.elem = hodnoty;
//	mat_print(&maticaA);
	
//	mat_unit(&maticaA);
//	mat_print(&maticaA);

//	mat_random(&maticaA);

//	mat_destroy(&maticaA);
//	mat_print(&maticaA);
//	mat_trace(&maticaA);
	
	//mat_save(&maticaA, "matica");
	//maticaB = mat_create_with_type(5,5);
	
	/*maticaB = mat_create_by_file("matica");
	printf("Riadky: %u\n", maticaB->rows);*/
	
}
