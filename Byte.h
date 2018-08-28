#ifndef __BYTE_H
#define __BYTE_H

//mask
#define BIT_I_SET(I) ((unsigned char)(1) << (I))
//const
#define SIZE_OF_BYTE 8
void saveCompressed(char *file_name, grayImage * image, unsigned char max_gray_level);
void convertCompressedImageToPGM(char *compressed_file_name, char *pgm_file_name);

#endif
