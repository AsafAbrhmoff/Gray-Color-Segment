#include "Struct.h"
#include "Segment.h"
#include "List.h"
#include "Byte.h"
#include "General.h"

static int howManyBits(unsigned char maxPixel);
static int isBitISet(BYTE ch, int i);
static void writeToPGMFile(FILE* file, unsigned int* counterOfPix, unsigned short colImage, unsigned char printChar, unsigned char maxGrayLevel);

void saveCompressed(char *file_name, grayImage * image, unsigned char max_gray_level)
//gets a GrayImage variable, compress it according to wanted maximum gray level user has defined
{
	//variables
	int numOfBits = howManyBits(max_gray_level);//calculate maximum number of bits
	int write = 0, countOfbit = 0;
	int i, j;
	unsigned char temp, tempCh = 0, tempzero = 0;
	unsigned short  row, col;

	FILE* file = fopen(file_name, "wb");//create file for writing
	checkOpenFile(file);//checks if file opening went well

	fwrite(&image->cols, sizeof(unsigned short), 1, file);//write number of colums into binary file
	fwrite(&image->rows, sizeof(unsigned short), 1, file);//write number of rows into binary file
	fwrite(&max_gray_level, sizeof(unsigned char), 1, file);//write maximum gray level into file

	for (i = 0; i < image->rows*image->cols; i++)//goes thourgh entire matrix of grayImage
	{
		row = i / image->cols;//calculate row number
		col = i - image->cols*row;//calculate colum number
		temp = (image->pixels[row][col] * max_gray_level) / 255;//calculate gray level of pixel
		for (j = 0; j <numOfBits; j++)//goes thourgh all bits of certain byte
		{
			if (isBitISet(temp, numOfBits - 1 - j) != 0)//checks if certain bit is different than 0
				tempCh |= BIT_I_SET(7 - write);//"lit" certain bit

			countOfbit++;//promote counter of bits
			if (countOfbit == numOfBits)//if program has gone thourgh all bits
			{
				countOfbit = 0;//zero counter
				if (write == 7)//if ch variable has been modified enough for writing
				{
					fwrite(&tempCh, sizeof(unsigned char), 1, file);//write into binary file
					tempCh = 0;//zero
					write = 0;//zero
				}//if
				else
					write++;//promote write
				break;//break loop
			}//if
			else if (write == 7)//if ch variable has been modified enough for writing
			{
				fwrite(&tempCh, sizeof(unsigned char), 1, file);//write into binary file
				write = 0;//zero counter
				tempCh = 0;//zero counter
			}//else if
			else
				write++;
		}//for
	}//for
	fwrite(&tempCh, sizeof(unsigned char), 1, file);//write ch into binary
	fclose(file);
}//saveCompressed
static int isBitISet(BYTE ch, int i)// checks if certain bit is one or zero, if zero, return zero
{
	BYTE mask = 1 << i;

	return ch & mask;
}//isBitISet
static int howManyBits(unsigned char maxPixel)//checks maximum number of bits needed for compressing
{
	int count = 0;
	int num = maxPixel;
	while (num > 0)
	{
		num /= 2;
		count++;
	}//while
	return count;
}//howManyBits
void convertCompressedImageToPGM(char *compressed_file_name, char *pgm_file_name)
//converts compressed binary file into PGM file
{
	//variables
	int numOfBits, size, reminder;
	int write, countOfPixels = 0, countOfBits = 0;
	unsigned char maxGrayLevel, temp, tempCh = 0;
	unsigned short rowsMat, colsMat;
	int i, j;
	char str[3] = { "P2" };

	FILE* binaryFile = fopen(compressed_file_name, "rb");//opens up file for reading
	checkOpenFile(binaryFile);//check if file opening went well
	FILE* pgmFile = fopen(pgm_file_name, "w");//creates PGM file for writing
	checkOpenFile(pgmFile);//checks if file opening went well

	fprintf(pgmFile, "%s\n", str);//writes "P2" into file
	fread(&colsMat, sizeof(unsigned short), 1, binaryFile);//gets number of colums
	fprintf(pgmFile, "%hu ", colsMat);//prints number of colums into PGM file
	fread(&rowsMat, sizeof(unsigned short), 1, binaryFile);//gets number of rows
	fprintf(pgmFile, "%hu\n", rowsMat);//prints number of rows into PGM file
	fread(&maxGrayLevel, sizeof(unsigned char), 1, binaryFile);//gets maximum gray level
	fprintf(pgmFile, "%d\n", 255);//prints maximum level of gray into PGM file

	numOfBits = howManyBits(maxGrayLevel);//calculate how many bits each byte is after compression
	size = colsMat*rowsMat*numOfBits;//calculate size of matrix for writing
	reminder = colsMat*rowsMat*numOfBits % SIZE_OF_BYTE;//calculate remainder of compression
	write = SIZE_OF_BYTE - numOfBits;//how many bits to modify before writing into PGM file

	if (reminder != 0)
		size = size + (SIZE_OF_BYTE - reminder);//add remainder to size

	for (i = 0; i < size; i = i + SIZE_OF_BYTE)//goes thourgh bytes
	{
		fread(&temp, sizeof(unsigned char), 1, binaryFile);//gets char variables from binary file
		for (j = 0; j <SIZE_OF_BYTE; j++)//goes thourgh bits of byte
		{
			if (isBitISet(temp, 7 - j) != 0)//checks which bits is different than 0
				tempCh |= BIT_I_SET(7 - write);//lits certain bits

			countOfBits++;//promote counter of bits
			if (countOfBits == SIZE_OF_BYTE)//if all bits have been checked
			{
				countOfBits = 0;//zero counter
				if (write != 7)//promote write counter
					write++;
				else
				{
					writeToPGMFile(pgmFile, &countOfPixels, colsMat, tempCh, maxGrayLevel);//writes into PGM file
					tempCh = 0;//zero char variable
					write = SIZE_OF_BYTE - numOfBits;//calculate number of bits
				}//else
				break;
			}//if
			if (write == 7)//if enough bits have been checked for writing
			{
				writeToPGMFile(pgmFile, &countOfPixels, colsMat, tempCh, maxGrayLevel);//write to PGM file
				tempCh = 0;//zero char variable
				write = SIZE_OF_BYTE - numOfBits;//calculate number of bits for next byte
			}//if
			else
				write++;
		}//for
	}//for
	fclose(pgmFile);//close for writing
	fclose(binaryFile);//close for writing
}//convertCompressedImageToPGM
static void writeToPGMFile(FILE* file, unsigned int* counterOfPix, unsigned short colImage, unsigned char printChar, unsigned char maxGrayLevel)
//write characters into PGM file
{
	printChar = (printChar * 255) / maxGrayLevel;//calculate which number to print
	(*counterOfPix)++;//promote counter of pixels
	if (*counterOfPix == colImage)//if end line
	{
		fprintf(file, "%d\n", printChar);
		*counterOfPix = 0;
	}//if

	else
		fprintf(file, "%d ", printChar);//print number into PGMfile
} //writeToPjmFile
