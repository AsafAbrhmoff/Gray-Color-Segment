#ifndef __SEGMENT_H
#define __SEGMENT_H

//Consts
#define SIZE_POSITION 2
//Positions on matrix
#define UPPER_LEFT_CORNER 1
#define UPPER_RIGHT_CORNER 2
#define LOWER_RIGHT_CORNER 3
#define LOWER_LEFT_CORNER 4
#define TOP_BORDER 5
#define LEFT_BORDER 6
#define BOTTOM_BORDER 7
#define RIGHT_BORDER 8
#define MIDDLE 9
#define CORNERS 10

Segment findSingleSegment(grayImage *img, imgPos start, unsigned char threshold);
void insertNodeToEndList(treeNodeListCell **list, treeNodeListCell *node);
int findAllSegments(grayImage *img, unsigned char threshold, imgPosList **segments);

#endif