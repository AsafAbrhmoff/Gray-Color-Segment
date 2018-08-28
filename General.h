#ifndef __GENERAL_H
#define __GENERAL_H

grayImage *readPGM(char *fname);
grayImage *colorSegments(imgPosList *segments, unsigned int size);
void freeDynamicMatrix(unsigned char **matrix, unsigned short rows, unsigned short cols);
unsigned char **allocateDynamicMatrix(unsigned short rows, unsigned short cols);
void checkOpenFile(FILE* file);
void checkMemoryAllocation(void *ptr);
void printMenu();
void freeGrayImage(grayImage *img);
#endif