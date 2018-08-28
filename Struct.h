#ifndef __STRUCT_H
#define __STRUCT_H

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#include <stdlib.h>


//forward declaration
typedef struct _treeNodeListCell treeNodeListCell;

//typedef structs
typedef struct _grayImage {
	unsigned short rows, cols;
	unsigned char **pixels;
}grayImage;

typedef unsigned short imgPos[2];

typedef struct _treeNode {
	imgPos position;
	treeNodeListCell *next_possible_positions;
}treeNode;

typedef struct _treeNodeListCell {
	treeNode *node;
	struct _treeNodeListCell *next;
}treeNodeListCell;

typedef struct _segment {
	treeNode *root;
}Segment;

typedef struct _imgPosCell {
	imgPos position;
	struct _imgPosCell *next, *prev;
}imgPosCell;

typedef struct _imgPosList {
	imgPosCell *head, *tail;
}imgPosList;

typedef int BOOL;
#define FALSE 0
#define TRUE 1

typedef unsigned char BYTE;



#endif