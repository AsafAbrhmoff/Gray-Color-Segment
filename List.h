#ifndef __LIST_H
#define __LIST_H

void freeArrayOfSegments(imgPosList *segments, unsigned int size);
void buildListFromSegment(imgPosList *lst, treeNode *root);
void makeEmptyListPos(imgPosList *lst);
#endif