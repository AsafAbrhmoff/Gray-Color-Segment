#include "Struct.h"
#include "Segment.h"
#include "List.h"
#include "Byte.h"
#include "General.h"


void main()
{
	BOOL picLoaded = FALSE;
	BOOL segmentFound = FALSE;
	BOOL notFinished = FALSE;
	grayImage *coloredSegments=NULL;
	grayImage *img = NULL;
	imgPosList *segmentList = NULL;
	unsigned int segmentListSize;
	int threshold;
	int maxGrayLevel;
	char fileName[FILENAME_MAX];
	char textFileName[FILENAME_MAX];
	char binFileName[FILENAME_MAX];
	char newFileName[FILENAME_MAX];
	char newColoredSegmentCompressed[FILENAME_MAX];
	int userSelection;
	printMenu();
	scanf("%d", &userSelection);
	printf("\n");


	do
	{
		switch (userSelection)
		{
		case(0):
		{
			printf("Bye!.");
			if(img != NULL)
				freeGrayImage(img);
			if(coloredSegments != NULL)
				freeGrayImage(coloredSegments);
			notFinished = TRUE;
			break;
		}
		case(1):
		{
			printf("Please provide a file name ===> ");
			scanf("%s", fileName);
			img = readPGM(fileName);
			picLoaded = TRUE;
			printMenu();
			scanf("%d", &userSelection);
			printf("\n");
			break;
		}
		case(2):
		{
			if (picLoaded == FALSE)
			{
				printf("Picture was no provided please provide a picture name to proceed.\n");
				printMenu();
				scanf("%d", &userSelection);
				printf("\n");
			}
			else
			{
				printf("Please provide a threshold for the segmentation build ===> ");
				scanf("%d", &threshold);
				segmentListSize = findAllSegments(img, (unsigned char)threshold, &segmentList);
				printMenu();
				scanf("%d", &userSelection);
				printf("\n");
				segmentFound = TRUE;
			}
			break;
		}
		case(3):
		{
			if (segmentFound == FALSE)
			{
				printf("Please find the segments first in order to proceed.\n\n");
				printMenu();
				scanf("%d", &userSelection);
				printf("\n");
			}
			else
			{
				coloredSegments = colorSegments(segmentList, segmentListSize);
				printMenu();
				scanf("%d", &userSelection);
				printf("\n");
			}
			break;
		}
		case(4):
		{
			printf("Please provide a new max gray level under the value of 127 for the compression ===> ");
			scanf("%d", &maxGrayLevel);
			printf("Please provide a new file name for the compressed result ===> ");
			scanf("%s", newColoredSegmentCompressed);
			saveCompressed(newColoredSegmentCompressed, coloredSegments, (unsigned char)maxGrayLevel);
			printMenu();
			scanf("%d", &userSelection);
			printf("\n");
			break;
		}
		case(5):
		{
			printf("Please provide a new max gray level under the value of 127 for the compression ===> ");
			scanf("%d", &maxGrayLevel);
			printf("Please provide a new file name ===> ");
			scanf("%s", newFileName);
			saveCompressed(newFileName, img, maxGrayLevel);
			printMenu();
			scanf("%d", &userSelection);
			printf("\n");
			break;
		}
		case(6):
		{
			printf("Please provide a compressed file name ===> ");
			scanf("%s", binFileName);
			printf("Please provide a new file name to write into ===> ");
			scanf("%s", textFileName);
			convertCompressedImageToPGM(binFileName, textFileName);
			printMenu();
			scanf("%d", &userSelection);
			printf("\n");
			break;
		}
		default:

			printf("Please provide legal input.\n");
			printMenu();
			scanf("%d", &userSelection);
			printf("\n");
			break;
		}
	} while (notFinished == FALSE);
}
