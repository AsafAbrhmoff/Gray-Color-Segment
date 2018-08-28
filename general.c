#include "Struct.h"
#include "Segment.h"
#include "List.h"
#include "Byte.h"
#include "General.h"

static void findRowCol(imgPosList *segments, unsigned int size, unsigned short *row, unsigned short *col);


grayImage *readPGM(char *fname)//gets filename of PGM file and opens it for reading
{
	//variables
	grayImage *res;
	unsigned short i, j;
	int maxGrayLevel;
	int tempNum;
	char str[3];
	res = (grayImage *)malloc(sizeof(grayImage));//memory allocation
	checkMemoryAllocation(res);//check memory allocation
	FILE *file = fopen(fname, "r");//open file for reading
	checkOpenFile(file);

	fgets(str, sizeof(str), file);//gets str of 3 characters
	fscanf(file, "%hu", &res->cols);//number of colums
	fscanf(file, "%hu", &res->rows);//number of rows
	fscanf(file, "%d", &maxGrayLevel);//maximum level of gray
	res->pixels = allocateDynamicMatrix(res->rows, res->cols);//allocating dynamic matrix

	for (i = 0; i < res->rows; i++)//for rows
	{
		for (j = 0; j < res->cols; j++)//for colums
		{
			fscanf(file, "%d", &tempNum);//getting value
			res->pixels[i][j] = (unsigned char)(tempNum);//inserting values
		}//for
	}//for
	fclose(file);//close reading
	return res;
}//readPGM
void checkOpenFile(FILE* file)
{
	if (file == NULL)
	{
		printf("Failed Opening The File. Exiting.");
		exit(2);
	}
}
unsigned char **allocateDynamicMatrix(unsigned short rows, unsigned short cols)
//creates a parallel matrix for verifying which cells have been checked
{
	//declaration of variables
	unsigned char **flagMatrix;
	unsigned short i; //indexes

	flagMatrix = (unsigned char**)calloc(rows, sizeof(unsigned char*)); //memory allocation
	checkMemoryAllocation(flagMatrix);//checking if memory allocation is successful

	for (i = 0; i < rows; i++)//allocating memory for each colum
	{
		flagMatrix[i] = (unsigned char*)calloc(cols, sizeof(unsigned char));//memory allocation
		checkMemoryAllocation(flagMatrix[i]);//checking if memory allocation is successful
	}//for
	return flagMatrix;
}//createFlagMatrix
void freeDynamicMatrix(unsigned char **matrix, unsigned short rows, unsigned short cols)
//frees matrix 
{
	unsigned short i;

	for (i = 0; i < rows; i++)
	{
		free(matrix[i]);//free lines of matrix
	}
	free(matrix);//free matrix
}//freeDynamicMatrix
void checkMemoryAllocation(void *ptr)//checks if memory allocation is successful
{
	if (ptr == NULL)
	{
		puts("Memory allocation error");
		exit(1);
	}//if
}//checkMemoryAllocation
grayImage *colorSegments(imgPosList *segments, unsigned int size)
//analyzes segments and matching gray level for each segment based on a formula
{
	//declaration of variables
	grayImage *res;
	unsigned short row = 0;
	unsigned short col = 0;
	imgPosCell *current;
	unsigned int i;
	float calc;

	res = (grayImage *)malloc(sizeof(grayImage));//memory allocation
	checkMemoryAllocation(res);//check memory allocation
	findRowCol(segments, size, &row, &col);//find maximum row and col thourgh link lists
	res->rows = row;//insert maximum value of row
	res->cols = col;//insert maximum value of colum

	res->pixels = allocateDynamicMatrix(res->rows, res->cols);//allocate matrix for inserting gray values

	for (i = 0; i < size; i++)//goes thourgh all linked lists of array
	{
		current = segments[i].head;
		while (current != NULL)//goes thourgh all nodes of list
		{
			calc = ((float)255 / (size - 1));
			row = current->position[0];//insert row number
			col = current->position[1];//insert colum number
			res->pixels[row][col] = (int)(i*(calc));//calculate level of gray
			current = current->next;//promote node
		}//while
	}//for
	 freeArrayOfSegments(segments, size);
	return res;
}//colorSegments
static void findRowCol(imgPosList *segments, unsigned int size, unsigned short *row, unsigned short *col)
//finds maximum row and colum number in order to recreate a fitting matrix
{
	//variables
	unsigned int i;
	imgPosCell *current;

	for (i = 0; i < size; i++)//goes thourgh all linked lists of array
	{
		current = segments[i].head;
		while (current != NULL)//goes thourgh all nodes 
		{
			if (current->position[0]>*row)//if row number in node is higher than current
				*row = current->position[0];//insert value
			if (current->position[1]>*col)//if colum number in node is higher than current
				*col = current->position[1];//insert value

			current = current->next;//promote node
		}//while
	}//for
	*row = *row + 1;
	*col = *col + 1;
}//findRowCol
void freeGrayImage(grayImage *img)//frees grayImage variable
{
	freeDynamicMatrix(img->pixels, img->rows, img->cols);//frees matrix
	free(img);//free GrayImage
}//freeGrayImage
void printMenu()
{
	printf("Please choose one of the following option to proceed :\n\n");
	printf("0. Exit\n");
	printf("1. Read an image file in PGM format\n");
	printf("2. Find all segments\n");
	printf("3. Color the segments\n");
	printf("4. Save the colored image in a compressed format\n");
	printf("5. Compress and save the original image in a compressed format\n");
	printf("6. Convert a compressed image to PGM format\n");
	printf("Please input your selection ===> ");
}
