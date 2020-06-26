#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//reprezentacia hustej matice pomocou struktury 
// row a cols mozu by len kladné cisla
//obovzdava adresu elementu 
typedef struct{
unsigned int rows;
unsigned int cols;
float *elem;
}MAT;

//makrom pristupujeme k prvkom matice 
#define ELEM(mat, i, j) (mat->elem[(i-1)* mat->cols + j - 1])           

//vytvori zakladnu datovu strukturu MAT
MAT *mat_create_with_type(unsigned int rows, unsigned int cols)
{
	MAT *ptr = (MAT*) malloc(sizeof(MAT));                //alokuje pamat na strukturu matice  
	ptr->elem = (float*) malloc(sizeof(float)*rows*cols); //alokuje pamat na vsetky elementy obsiahnute v matici
	
	if(ptr == NULL)   //test, ci alokacia prebehla uspesne                
    {
        printf("Error! nebola pridelena pamat.");
        free(ptr);   //uvolni vsetku dynamicky alokovanu pamat 
    }
	return ptr; 	
}

//inicialuzuje sa datova struktura MAT
//vytvori hustu reprezentaciu matice v pamati
//inicializuje hodnoty matice ktore nacita zo suboru
MAT *mat_create_by_file(char *filename)
{
	FILE *ptr;
	ptr = fopen(filename,"rb");	// r for read, b for binary
	
	char mat, typ;
	unsigned int rows, cols; 
	
	printf("Nacitavame maticu zo suboru: %s\n", filename);

	
	fread(&mat,sizeof(mat),1,ptr);	
	fread(&typ,sizeof(typ),1,ptr); 
	fread(&rows,sizeof(rows),1,ptr);
	fread(&cols,sizeof(cols),1,ptr);
	
//	printf("Nacitali sme schemu: %c %c %u %u\n", mat, typ, rows, cols);
//	printf("Nacitali sme elementy:\n");
	
	MAT *matica;
	matica = mat_create_with_type(rows,cols);
	matica->rows = rows;
	matica->cols = cols;

//	printf("Matica v programe je uz inicializovana a ma uz udaje o velkosti: %d a %d\n", matica->rows, matica->cols);
	for (int i=0;i < (matica->rows * matica->cols);i++) 
	{
    //	printf("Element ulozeny do inicializovanej matice: %lf\n",matica->elem[i]);
    	fread(&matica->elem[i],sizeof(matica->elem),1,ptr);
	}
	
	return  matica;
}

//funkcia odovzdá adresu miesta kde sa struktura matice nachadza 
char mat_save(MAT *mat, char *filename)
{
	char chyba = 0;
	FILE *write_ptr;
	write_ptr = fopen(filename,"wb");  // w for write, b for binary
	if(write_ptr == NULL){
		printf("Chyba pri otvarani suboru: %s\n", filename);
		return 1;
	}
	
	printf("Ukladanie Matice do suboru: %s\n", filename);
//	printf("Rows: %d ", &mat->rows);
//	printf("Cols: %d ", &mat->cols);
//	printf("Size of Mat->elem: %d \n", sizeof(mat->elem));
	
	chyba =(char) fwrite("M",sizeof(char),1,write_ptr);
	if(chyba != 1)
	{
		printf("chyba pri ukladani do suboru pri M");
		return chyba;
	}

	chyba =(char) fwrite("1",sizeof(char),1,write_ptr);
	if(chyba != 1)
	{
		printf("chyba pri ukladani do suboru pri 1");
		return chyba;
	}

	chyba =(char) fwrite(&mat->rows,sizeof(unsigned int),1,write_ptr);
	if(chyba != 1)
	{
		printf("chyba pri ukladani do suboru pri ROWS");
		return chyba;
	}


	chyba =(char) fwrite(&mat->cols,sizeof(unsigned int),1,write_ptr);
	if(chyba != 1)
	{
		printf("chyba pri ukladani do suboru pri COLS");
		return chyba;
	}

	for(int i = 0; i<(mat->rows*mat->cols);i++)	
		{
	//	printf("Ukladam do suboru element: %f\n", mat->elem[i]);
		chyba =(char) fwrite(&mat->elem[i],sizeof(mat->elem),1,write_ptr);
		if(chyba != 1)
		{
			printf("chyba pri ukladani do suboru pri ELEM");
			return chyba;
		}

		}
	fclose(write_ptr);
	return chyba;

}

//uvolni veetku pamat, ktora sa podiela na reprezentacii matice mat 
void mat_destroy(MAT *mat){
	free(mat);
	free(mat->elem);
}


void mat_unit(MAT *mat)
{
//	printf("Spusta sa mat_unit\n");
	for(int i=0;i<(mat->rows);i++)
	{
		for(int j=0;j<(mat->cols);j++)
		{
			if(i==j){
				//printf("nasiel diagonalu %d %d", i + 1 , j + 1);
				ELEM(mat, i + 1, j + 1) = 1;
				//printf("%f\t",mat->elem[i*mat->cols + j]);
			}
			if(i!=j)
				ELEM(mat, i + 1, j + 1) = 0; 
		}	
	}
}

void mat_random(MAT *mat)
{
//	printf("Spusta sa mat_random\n");
	srand(time(NULL));
//	printf("Hodnota Rows: %d\n", mat->rows);
//	printf("Hodnota Cols: %d\n", mat->cols);
//	mat_print(mat);
	for(int i=0;i<(mat->rows);i++)
	{
		for(int j=0;j<(mat->cols);j++)
		{
		//	printf("pristupujeme k elementu na pozicii %d,%d a menime ho: ", i, j);
		//	printf("pristup cez maticu je: %f", mat->elem[(i)* mat->cols + j]);
		//	printf("%f \n",ELEM(mat, i + 1, j + 1));
			//vypocet random cisla z rozshahu -1 a 1 
			//float(rand()) / (float)(RAND_MAX)) * (MAX - MIN) + MIN
			ELEM(mat, i + 1, j + 1) = (float(rand()) / (float)(RAND_MAX)) * ( 1 - (-1)) + (-1); 
		}	
	}

}

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
				//printf("%f\t",stopa);
			}
			
		}	
	}
	return stopa;
}

main() 
{
/*definované hodoty elementov v matici
	MAT maticaA;
	float hodnoty[] = {1,3,3,8,2,2,6,2,4,4,7,3,1,1,5,9};
	maticaA.rows = 4;
	maticaA.cols = 4;
	maticaA.elem = hodnoty;
	
	printf("Riadky: %u\n", maticaA.rows);
	printf("Stlpce: %u\n", maticaA.cols);
	for (int i=0;i < (maticaA.rows * maticaA.cols);i++) 
	{
    	printf("%lf\n",hodnoty[i]);
	}
	
	mat_unit(&maticaA);
	mat_print(&maticaA);
	mat_random(&maticaA);
	mat_print(&maticaA);
	printf("Stopa matice : %f",mat_trace(&maticaA));
	mat_save(&maticaA, "matica");
*/

/*
	MAT *maticaB;
	char save = 0;
	//nahodne generovana matica 7x7
	maticaB = mat_create_with_type(7, 7); // inicializacia struktury v pamati
	maticaB->rows = 7; // manualne nastavenie hodnoty rows
	maticaB->cols = 7; // manualne nastavenie hodnoty cols
	printf("hodnoty matice po inicializacii\n");
	printf("Velkost alokovanej pamate pre maticu: %d\n", sizeof(maticaB));
	printf("Rows: %d\n", maticaB->rows);
	printf("Cols: %d\n", maticaB->cols);
	printf("Velkost pola Elem: %d\n", sizeof(maticaB->elem)/sizeof(maticaB->elem[0]));//
	mat_random(maticaB); // funkcia automaticky vygeneruje nahodne hodnoty pre elementy
	mat_print(maticaB);
	printf("Hodnota elementu je: %f\n", ELEM(maticaB, 2, 2));
	printf("V main pred spustenim SAVE\n");
	save = mat_save(maticaB, "maticaB");
	printf("vystupna hodnota z funkcie SAVE: %c\n", save);
	mat_destroy(maticaB);
*/
	
/*	MAT *maticaD;
	maticaB = mat_create_by_file("maticaD");
	printf("Hodnoty MaticaC\n");
	printf("Rows: %d\n", maticaD->rows);
	printf("Cols: %d\n", maticaD->cols);
	mat_print(maticaD);
	mat_create_with_type(maticaD->rows,maticaD->cols);
	printf("Stopa matice : %f",mat_trace(maticaD));
	mat_destroy(maticaD);
*/

	unsigned int r,c;

	MAT* maticaC;
	
	printf("Zadaj pocet riadkov a pocet stlpcov:\n");
	scanf("%d %d",&r,&c);
	maticaC = mat_create_with_type(r,c);
	maticaC -> rows = r;
	maticaC -> cols = c;
	
	mat_unit(maticaC);
	mat_print(maticaC);
 	mat_random(maticaC);
	mat_print(maticaC);	
	
	printf("Stopa matice : %f\n",mat_trace(maticaC));
	
	mat_save(maticaC, "maticaC");
	mat_create_by_file("maticaC");
	mat_print(maticaC);
	
	mat_destroy(maticaC);
	
}

