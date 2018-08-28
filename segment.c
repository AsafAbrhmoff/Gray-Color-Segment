#include "Struct.h"
#include "Segment.h"
#include "List.h"
#include "Byte.h"
#include "General.h"

static treeNodeListCell* buildSegment(grayImage *img, treeNode *root, unsigned char **flagMatrix, unsigned char threshold, imgPos mainStart);
static unsigned short locationInMatrix(unsigned short rows, unsigned short cols, imgPos start);
static void updateFlagMatrix(imgPos start, unsigned char** flagMatrix);
static void startPosition(grayImage *img, imgPos start, imgPos pos);
static treeNodeListCell *buildList(grayImage *img, imgPos pos, unsigned char **flagMatrix, unsigned char threshold, imgPos mainStart);
static BOOL ifInRange(grayImage *img, imgPos pos, imgPos rootPos, unsigned char threshold);
static treeNode* createTreeNode(imgPos pos, treeNodeListCell *lst);
static void insertDataToEndList(treeNodeListCell **list, treeNode *node);
static void freeSegment(treeNode *node);
static treeNodeListCell* createListNode(treeNode *node, treeNodeListCell *next);
static BOOL ifCorners(unsigned short location); static treeNodeListCell* searchNeighboursOfLineMatrix(grayImage *img, imgPos pos, imgPos mainStart, unsigned char ** flagMatrix, unsigned char threshold);
static treeNodeListCell* searchNeighbours(grayImage *img, imgPos start, imgPos temp, imgPos pos, imgPos mainStart, unsigned char ** flagMatrix, unsigned char threshold, int row, int col);
static treeNodeListCell* searchNeighboursOfColumnMatrix(grayImage *img, imgPos pos, imgPos mainStart, unsigned char ** flagMatrix, unsigned char threshold);
static treeNodeListCell* searchNeighboursOfLineMatrix(grayImage *img, imgPos pos, imgPos mainStart, unsigned char ** flagMatrix, unsigned char threshold);
static void checkConditionsForNeighbours(treeNodeListCell **list, grayImage *img, imgPos pos, imgPos mainStart, unsigned char ** flagMatrix, unsigned char threshold);

Segment findSingleSegment(grayImage *img, imgPos start, unsigned char threshold)
//Auxiliary function that manages all function in order to create a segment
{
	Segment res;
	unsigned char **flagMatrix;

	res.root = createTreeNode(start, NULL);//Creates root's treeNode
	flagMatrix = allocateDynamicMatrix(img->rows, img->cols);
	updateFlagMatrix(start, flagMatrix);//updates flag matrix

	res.root->next_possible_positions = buildSegment(img, res.root, flagMatrix, threshold, start);//build segment
	freeDynamicMatrix(flagMatrix, img->rows, img->cols);//free matrix

	return res;
}//findSingleSegment
static treeNodeListCell* buildSegment(grayImage *img, treeNode *root, unsigned char **flagMatrix, unsigned char threshold, imgPos mainStart)
//creates a segment
{
	treeNodeListCell *newList;
	treeNodeListCell *current;

	newList = buildList(img, root->position, flagMatrix, threshold, mainStart);//creates list of "neighbours" of certain cell by threshold value
	current = newList;//inserting address of list

	if (newList == NULL)//stopping condition
	{
		return newList;
	}//if
	else
	{
		while (current != NULL)//goes thourgh all possible neighbours of nodes
		{
			current->node->next_possible_positions = buildSegment(img, current->node, flagMatrix, threshold, mainStart);
			current = current->next;//promoting
		}//while
	}//else
	return newList;
}//buildSegment
static unsigned short locationInMatrix(unsigned short rows, unsigned short cols, imgPos start)
//analyzes cell's location and return short value according to it
{
	if (start[0] == 0 && start[1] == 0)
		return UPPER_LEFT_CORNER;
	else if (start[0] == 0 && start[1] == (cols - 1))
		return UPPER_RIGHT_CORNER;
	else if (start[0] == (rows - 1) && start[1] == (cols - 1))
		return LOWER_RIGHT_CORNER;
	else if (start[0] == (rows - 1) && start[1] == 0)
		return LOWER_LEFT_CORNER;
	else if (start[0] == 0)
		return TOP_BORDER;
	else if (start[1] == 0)
		return LEFT_BORDER;
	else if (start[0] == (rows - 1))
		return BOTTOM_BORDER;
	else if (start[1] == (cols - 1))
		return RIGHT_BORDER;
	else
		return MIDDLE;
}//locationInMatrix
static void updateFlagMatrix(imgPos start, unsigned char** flagMatrix)
{
	flagMatrix[start[0]][start[1]] = 1;
}
static void startPosition(grayImage *img, imgPos start, imgPos pos)
//analyzes cell's location on matrix and according to it sends a location where to start searching
{
	unsigned short location = locationInMatrix(img->rows, img->cols, pos);//searches for correct search starting point

																		  //insert values of search starting point according cell's location
	if (location == UPPER_LEFT_CORNER)
	{
		start[0] = pos[0];
		start[1] = pos[1];
	}//if
	else if (location == UPPER_RIGHT_CORNER || location == TOP_BORDER)
	{
		start[0] = pos[0];
		start[1] = pos[1] - 1;
	}//else if
	else if (location == LOWER_RIGHT_CORNER || location == BOTTOM_BORDER || (location == RIGHT_BORDER) || location == MIDDLE)
	{
		start[0] = pos[0] - 1;
		start[1] = pos[1] - 1;
	}//else if
	else if (location == LOWER_LEFT_CORNER || location == LEFT_BORDER)
	{
		start[0] = pos[0] - 1;
		start[1] = pos[1];
	}//else if
}//startPosition
static treeNodeListCell *buildList(grayImage *img, imgPos pos, unsigned char **flagMatrix, unsigned char threshold, imgPos mainStart)
//build list of neighbours according to a certain cell
{
	BOOL ifInCorner;
	imgPos start, temp;
	treeNodeListCell *newList = NULL;
	if (img->rows == 1)
		newList = searchNeighboursOfLineMatrix(img, pos, mainStart, flagMatrix, threshold);
	else if (img->cols == 1)
		newList = searchNeighboursOfColumnMatrix(img, pos, mainStart, flagMatrix, threshold);
	else
	{
		unsigned short location = locationInMatrix(img->rows, img->cols, pos);//gets the location of a cell in a matrix
		startPosition(img, start, pos);//sends a location of where to start searching
		temp[0] = start[0];//inserting values of search starting point
		temp[1] = start[1];//inserting values of search starting point
						   //if location == corners
		ifInCorner = ifCorners(location);
		if (ifInCorner == TRUE)
		{
			newList = searchNeighbours(img, start, temp, pos, mainStart, flagMatrix, threshold, 2, 2);
		}//if
		else if (location == MIDDLE)//if location == Middle
		{
			newList = searchNeighbours(img, start, temp, pos, mainStart, flagMatrix, threshold, 3, 3);
		}//else if
		else if (location == TOP_BORDER || location == BOTTOM_BORDER)//if location is at top or bottom(not corners)
		{
			newList = searchNeighbours(img, start, temp, pos, mainStart, flagMatrix, threshold, 2, 3);
		}//else if
		else//if location == RIGHT_BORDER or LEFT_BORDER
		{
			newList = searchNeighbours(img, start, temp, pos, mainStart, flagMatrix, threshold, 3, 2);
		}//else
	}
	return newList;
}//buildList
static BOOL ifCorners(unsigned short location)
{
	if (location == UPPER_LEFT_CORNER || location == UPPER_RIGHT_CORNER || location == LOWER_RIGHT_CORNER || location == LOWER_LEFT_CORNER)
		return TRUE;

	return FALSE;
}
static treeNodeListCell* searchNeighbours(grayImage *img, imgPos start, imgPos temp, imgPos pos, imgPos mainStart, unsigned char ** flagMatrix, unsigned char threshold, int row, int col)
{
	treeNodeListCell *newList = NULL;
	int line, colum;
	for (line = 0; line < row; line++)//goes thourgh wanted lines
		for (colum = 0; colum < col; colum++)//goes thourgh wanted colums
		{
			start[0] = temp[0] + line;//inserting values
			start[1] = temp[1] + colum;//inserting values
									   //condition for making sure the same cell isn't being checked
			if ((start[0] != pos[0] || start[1] != pos[1]) && (start[0] != mainStart[0] || start[1] != mainStart[1]))
				checkConditionsForNeighbours(&newList, img, start, mainStart, flagMatrix, threshold);
		}//for
	return (newList);
}//searchNeighbours
static treeNodeListCell* searchNeighboursOfLineMatrix(grayImage *img, imgPos pos, imgPos mainStart, unsigned char ** flagMatrix, unsigned char threshold)
{
	treeNodeListCell *newList = NULL;
	imgPos temp;
	if (pos[1] == 0)
	{
		temp[0] = pos[0];
		temp[1] = pos[1] + 1;
		checkConditionsForNeighbours(&newList, img, temp, mainStart, flagMatrix, threshold);
	}
	else if (pos[1] == img->cols - 1)
	{
		temp[0] = pos[0];
		temp[1] = pos[1] - 1;
		checkConditionsForNeighbours(&newList, img, temp, mainStart, flagMatrix, threshold);
	}
	else
	{
		temp[0] = pos[0];
		temp[1] = pos[1] - 1;
		checkConditionsForNeighbours(&newList, img, temp, mainStart, flagMatrix, threshold);
		temp[0] = pos[0];
		temp[1] = pos[1] + 1;
		checkConditionsForNeighbours(&newList, img, temp, mainStart, flagMatrix, threshold);
	}

	return (newList);
}//searchCorners
static treeNodeListCell* searchNeighboursOfColumnMatrix(grayImage *img, imgPos pos, imgPos mainStart, unsigned char ** flagMatrix, unsigned char threshold)
{
	treeNodeListCell *newList = NULL;
	imgPos temp;
	if (pos[0] == 0)
	{
		temp[0] = pos[0] + 1;
		temp[1] = pos[1];
		checkConditionsForNeighbours(&newList, img, temp, mainStart, flagMatrix, threshold);
	}
	else if (pos[0] == img->rows - 1)
	{
		temp[0] = pos[0] - 1;
		temp[1] = pos[1];
		checkConditionsForNeighbours(&newList, img, temp, mainStart, flagMatrix, threshold);
	}
	else
	{
		temp[0] = pos[0] - 1;
		temp[1] = pos[1];
		checkConditionsForNeighbours(&newList, img, temp, mainStart, flagMatrix, threshold);
		temp[0] = pos[0] + 1;
		temp[1] = pos[1];
		checkConditionsForNeighbours(&newList, img, temp, mainStart, flagMatrix, threshold);
	}

	return (newList);
}//searchCorners
static void checkConditionsForNeighbours(treeNodeListCell **list, grayImage *img, imgPos pos, imgPos mainStart, unsigned char ** flagMatrix, unsigned char threshold)
{
	treeNode *newNode;
	if (flagMatrix[pos[0]][pos[1]] == 0)//condition for checking if cell has been checked
		if (ifInRange(img, pos, mainStart, threshold) == TRUE)//checks if start's value fits threshold gap
		{
			newNode = createTreeNode(pos, NULL);//creates new node
			updateFlagMatrix(pos, flagMatrix);//updares flag matrix that cell has been searched
			insertDataToEndList(list, newNode);//inserts TreeNodeListCell to end of list
		}//if
}
static BOOL ifInRange(grayImage *img, imgPos pos, imgPos rootPos, unsigned char threshold)
//checks if color values fits thershold gap
{
	int subtraction;
	subtraction = img->pixels[rootPos[0]][rootPos[1]] - img->pixels[pos[0]][pos[1]];//subtracting values
	if ((subtraction <= threshold) && (subtraction >= (-1)*threshold))//if values is in range
		return TRUE;
	else//if value doesn't fit
		return FALSE;
}//ifInRange
 //the function create new node and return him by pointer
static treeNode* createTreeNode(imgPos pos, treeNodeListCell *lst)//Create treeNodes
{
	//declaration of variables
	treeNode *res;
	unsigned short i;

	res = (treeNode*)malloc(sizeof(treeNode));//Memory allcation
	checkMemoryAllocation(res);//checking if allocation is successful

	for (i = 0; i < SIZE_POSITION; i++)//loop that inserts line and colum numbers
	{
		res->position[i] = pos[i];
	}//for
	res->next_possible_positions = lst;//inserting TreeNodeListCell adresses
	return res;
}//createTreeNode
 //the function enter a new data to the end of the list
static void insertDataToEndList(treeNodeListCell **list, treeNode *node)
{
	//declaration of variables
	treeNodeListCell *newTail;
	newTail = createListNode(node, NULL); //create new data
	insertNodeToEndList(list, newTail);
}
//the function enter the new node to the end of the list
void insertNodeToEndList(treeNodeListCell **list, treeNodeListCell *node)
//inserts adress of TreeNodeListCell
{
	treeNodeListCell *current;
	if (*list == NULL)//if certain cell has no neighbours
	{
		*list = node;
	}//if
	else//if cell has neighbours
	{
		current = *list;//inserting addresses
		while (current->next != NULL)//goes thourgh all TreeNodeListCell
			current = current->next;//promote TreeNodeListCell
		current->next = node;//inserting adress
	}//else
}//insertNodeToEndList
 //the function create new node to the list
static treeNodeListCell* createListNode(treeNode *node, treeNodeListCell *next)
//creates treeNodeListCell
{
	//declaration of variables
	treeNodeListCell *result;

	result = (treeNodeListCell *)malloc(sizeof(treeNodeListCell));//memory allocation
	checkMemoryAllocation(result);//checks if memory allocation is successful

	result->node = node;//inserting addresses
	result->next = next;//inserting adresses
	return result;
}//createListNode
int findAllSegments(grayImage *img, unsigned char threshold, imgPosList **segments)//find all segments and creates an array 
																				   //of linked lists of imgPos
{
	//variables
	Segment res;
	int  logicSize = 0, physicalSize = 1;
	imgPosList *lst;
	unsigned char **flagMatrix;
	imgPos ip;
	unsigned short i, j;

	*segments = (imgPosList *)malloc(physicalSize * sizeof(imgPosList));//memory allocation of array for linked lists
	checkMemoryAllocation(segments);//memory allocation check

	flagMatrix = allocateDynamicMatrix(img->rows, img->cols);//creates a parllel matrix for checking which cells have been checked

	for (i = 0; i < img->rows; i++)//goes thourgh rows
		for (j = 0; j < img->cols; j++)//goes thourgh colums
		{
			if (flagMatrix[i][j] == 0)//if cell hasn't been checked
			{
				if (logicSize == physicalSize)//making sure there's enough space in the array
				{
					physicalSize *= 2; //increase the space
					*segments = (imgPosList *)realloc(*segments, physicalSize * sizeof(imgPosList));//double memory capacity
					checkMemoryAllocation(segments);//memory allocation check
				}//if
				ip[0] = i;//insert row value
				ip[1] = j;//insert colum value
				lst = (imgPosList *)malloc(sizeof(imgPosList));//memory allocation
				checkMemoryAllocation(lst);//memory allocation check
				res.root = createTreeNode(ip, NULL);//Creates root's treeNode
				updateFlagMatrix(ip, flagMatrix);//updates flag matrix
				res.root->next_possible_positions = buildSegment(img, res.root, flagMatrix, threshold, ip);//recourse that build a segemnt
				makeEmptyListPos(lst);//empties list
				buildListFromSegment(lst, res.root);//build linked list out of segment
				(*segments)[logicSize] = *lst;//insert values
				logicSize++;//increse actual size
				freeSegment(res.root);
			}//if
		}//for
	*segments = (imgPosList *)realloc(*segments, logicSize * sizeof(imgPosList));//allocate exact size needed
	freeDynamicMatrix(flagMatrix, img->rows, img->cols);//free flag matrix
	return logicSize;//return actual size
}//findAllSegments
static void freeSegment(treeNode *node)//recourse that frees segment after creating a linked list out of it
{
	//variables
	treeNodeListCell *cell, *temp;
	if (node == NULL)//stopping condition
	{
		return;
	}//if
	else// node!= NULL
	{
		cell = node->next_possible_positions;
		while (cell != NULL)//goes thourgh all next_possible_positions
		{
			freeSegment(cell->node);//call recourse function
			temp = cell;//insert adress
			cell = cell->next;//promote cell
			free(temp);//free previous cell
		}//while
		free(node);//free node
	}//else
}//freeSegment