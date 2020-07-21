#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

//reprezentacia hustej matice pomocou struktury 
// row a cols mozu byt len kladne cisla
//obovzdava adresu elementu 
typedef struct{
unsigned int rows;
unsigned int cols;
float *elem;
}MAT;

//makrom pristupujeme k prvkom matice 
#define ELEM(mat, i, j) (mat->elem[(i-1)* mat->cols + j - 1])           

MAT *mat_create_with_type(unsigned int rows, unsigned int cols);
MAT *mat_create_by_file(char *filename);
char mat_save(MAT *mat, char *filename);
void mat_destroy(MAT *mat);
void mat_unit(MAT *mat);
void mat_random(MAT *mat);
void mat_print(MAT *mat);

char mat_trace(MAT *mat,float *trace);

//vytvori zakladnu datovu strukturu MAT
MAT *mat_create_with_type(unsigned int rows, unsigned int cols)
{
	MAT *ptr = (MAT*) malloc(sizeof(MAT));                //alokuje pamat na strukturu matice  
	if(ptr == NULL)   									 //test, ci alokacia prebehla uspesne                
    {
        free(ptr);   									//uvolni vsetku dynamicky alokovanu pamat 
        return NULL;
    }
    
	ptr->elem = (float*) malloc(sizeof(float)*rows*cols); //alokuje pamat na vsetky elementy obsiahnute v matici
	if(ptr->elem == NULL)   									 //test, ci alokacia prebehla uspesne                
    {					 
        mat_destroy(ptr);	//uvolni vsetku dynamicky alokovanu pamat 
        return NULL;
    }
	ptr ->rows = rows; 
	ptr ->cols = cols;

	return ptr; 	
}

//inicialuzuje sa datova struktura MAT
//vytvori hustu reprezentaciu matice v pamati
//inicializuje hodnoty matice ktore nacita zo suboru
MAT *mat_create_by_file(char *filename)
{	
	int fd = -1;
	if ((fd = open(filename , O_BINARY | O_RDONLY)) < 0 ) 
	{
		fprintf( stderr,"File access problem.\n");
		exit(1);
	}

	//unsigned int count = 0;
	char mat, typ;
	unsigned int rows = 0, cols = 0; 
	
	read(fd, &mat, sizeof(mat));   
	if (mat != 'M' ) 
	{
		fprintf( stderr,"Subor neobsahuje ako prvy znak M. Nezadal si mi subor s maticou. Prvy znak ktory sa nacital bol: %c\n",mat);
		exit(1);
	}

	read(fd, &typ, sizeof(typ));
		if (typ != '1' ) 
	{
		fprintf( stderr,"Subor neobsahuje ako prvy znak M. Nezadal si mi subor s maticou. Prvy znak ktory sa nacital bol: %c\n",typ);
		exit(1);
	}
	read(fd, &rows, sizeof(rows));
	read(fd, &cols, sizeof(cols));
	
	MAT *matica;
	matica = mat_create_with_type(rows,cols);
	matica->rows = rows;
	matica->cols = cols;
	
	for (int i=0; i < (matica->rows * matica->cols); i++) 
	{
    	read(fd, &matica->elem[i], sizeof(matica->elem));
	}
	
	close(fd);
	return  matica;
}

//funkcia odovzda adresu miesta kde sa struktura matice nachadza 
char mat_save(MAT *mat, char *filename)
{
	int fd = -1;
	unsigned int chyba = 0; 
	
	if ((fd = open(filename , O_BINARY | O_WRONLY )) < 0 ) 
	{
		fprintf( stderr,"File access problem.\n");
		exit(1);
	}
	
	chyba =	write(fd,"M", sizeof(char));
	if(chyba != sizeof(char)) //chyba pri ukladani do suboru pri M
		return chyba;
		
	chyba =	write(fd,"1", sizeof(char));
	if(chyba != sizeof(char)) //chyba pri ukladani do suboru pri 1
		return chyba;
		
	chyba =	write(fd,&mat->rows, sizeof(unsigned int));
	if(chyba != sizeof(unsigned int)) //chyba pri ukladani do suboru pri ROWS
		return chyba;
		
	chyba =	write(fd,&mat->cols, sizeof(unsigned int));
	if(chyba != sizeof(unsigned int))  //chyba pri ukladani do suboru pri COL
		return chyba;

	for(int i = 0; i<(mat->rows*mat->cols);i++)	
	{
		chyba =	write(fd,&mat->elem[i], sizeof(float));
		if(chyba != sizeof(mat->elem[i])) //chyba pri ukladani do suboru pri ELEM
			return chyba;
	}
	close(fd);
	return chyba;
}

//uvolni veetku pamat, ktora sa podiela na reprezentacii matice mat 
void mat_destroy(MAT *mat){
	free(mat->elem);
	free(mat);
}


void mat_unit(MAT *mat)
{
	for(int i=0;i<(mat->rows);i++)
	{
		for(int j=0;j<(mat->cols);j++)
		{
			if(i==j){
				ELEM(mat, i + 1, j + 1) = 1;
			}
			if(i!=j)
				ELEM(mat, i + 1, j + 1) = 0; 
		}	
	}
}

void mat_random(MAT *mat)
{
	for(int i=0;i<(mat->rows);i++)
	{
		for(int j=0;j<(mat->cols);j++)
		{
			//float(rand()) / (float)(RAND_MAX)) * (MAX - MIN) + MIN
			ELEM(mat, i + 1, j + 1) = (float(rand()) / (float)(RAND_MAX)) * ( 1 - (-1)) + (-1); 
		}	
	}
}

void mat_print(MAT *mat)
{
	for(int i=0;i<(mat->rows);i++)
	{
		for(int j=0;j<(mat->cols);j++)
			printf("%f\t",mat->elem[i*mat->cols + j]);
		printf("\n");
	}
}	

char mat_trace(MAT *mat, float *trace)
{
	float stopa=0;
	char chyba = 0;
	if (mat->cols != mat->rows) 
			return 1;
			
	for(int i=0;i<(mat->rows);i++)
	{
		for(int j=0;j<(mat->cols);j++)
		{
			if(i==j)
				stopa += ELEM(mat, i + 1, j + 1);
			
		}	
	}	
	*trace = stopa; 

	return chyba;
}

main() 
{
	MAT* maticaC;
	float stopa;
	
	srand(time(NULL));

	maticaC = mat_create_with_type(4,4);
	mat_random(maticaC);
	mat_print(maticaC);
	printf("\n");
	if( mat_trace(maticaC,&stopa) == 1)
		printf("Matica nie je NxN");
	else 	
		printf("Stopa matice : %f\n",stopa);

	mat_destroy(maticaC);

}

