#include "Struct.h"
#include "Segment.h"
#include "List.h"
#include "Byte.h"
#include "General.h"

static int isEmptyListPos(imgPosList lst);
static void insertDataToEndListPos(imgPosList *lst, imgPos position);
static void insertNodeToEndListPos(imgPosList* lst, imgPosCell *tail);
static imgPosCell* createImgPosCellNode(imgPos position, imgPosCell *prev, imgPosCell *next);
static void freeList(imgPosList lst);


void buildListFromSegment(imgPosList *lst, treeNode *root)//recourse that builds list out of segment
{
	treeNodeListCell *current;

	insertDataToEndListPos(lst, root->position);//inserts data to end of list

	if (root->next_possible_positions == NULL)//if number of "neighbours" is zero
	{
		return;
	}//if
	else
	{
		current = root->next_possible_positions;
		while (current != NULL)
		{
			buildListFromSegment(lst, current->node);//call recourse function
			current = current->next;//promote node
		}//while
	}//else
}
void makeEmptyListPos(imgPosList *lst)//empties list
{
	lst->head = lst->tail = NULL;
}//makeEmptyListPos
static int isEmptyListPos(imgPosList lst)//checks if list is empty
{
	return (lst.head == NULL);
}//isEmptyListPos
static void insertDataToEndListPos(imgPosList *lst, imgPos position)//create and enters node to end of list
{
	//declaration of variables
	imgPosCell *newTail;
	newTail = createImgPosCellNode(position, lst->tail, NULL); //create new node
	insertNodeToEndListPos(lst, newTail);//insert node to end of list
}//insertDataToEndListX
static void insertNodeToEndListPos(imgPosList* lst, imgPosCell *tail)//inserts node to end of list
{
	if (isEmptyListPos(*lst))//if list is empty
	{
		tail->next = tail->prev = NULL;
		lst->head = lst->tail = tail;
	}//if
	else //if not
	{
		lst->tail->next = tail;
		lst->tail = tail;
	}//else
}//insertNodeToEndListPos
static imgPosCell* createImgPosCellNode(imgPos position, imgPosCell *prev, imgPosCell *next)//create new imgPos node
{
	//declaration of variables
	imgPosCell *result;
	int i;

	result = (imgPosCell *)malloc(sizeof(imgPosCell));//memory allocation
	checkMemoryAllocation(result);//checking memory allocation

	for (i = 0; i < SIZE_POSITION; i++)//loop that inserts line and colum numbers
	{
		result->position[i] = position[i];//insert values of position
	}//for

	result->next = next;//enter next adress
	result->prev = prev;//enter prev adress
	return result;
}//createImgPosCellNode
void freeArrayOfSegments(imgPosList *segments, unsigned int size)
//frees array of segments
{
	unsigned int i;

	for (i = 0; i < size; i++)
		freeList(segments[i]);//free each segment

	free(segments);//free array

}//freeArrayOfSegments
static void freeList(imgPosList lst)//freeList
{
	//declaration of variables
	imgPosCell *current;
	imgPosCell *next;
	current = lst.head;

	while (current != NULL)//goes thourgh all nodes and frees them
	{
		next = current->next;
		free(current);
		current = next;
	}//while
	lst.head = NULL;
	lst.tail = NULL;
}//freeList