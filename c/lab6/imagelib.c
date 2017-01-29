#include <stdio.h>
#include <stdlib.h>
#include "image.h"

read_error_code_t from_bmp_image(FILE* in, image_t* const img)
{
	int i, trash_len;
	size_t string_length;

	if(fread(&img->header, sizeof(bmp_header_t), 1, in) != 1)
		return READ_INVALID_HEADER;

	if(img->header.bftype != 19778)
		return READ_INVALID_SIGNATURE;
    
    img->data = malloc(sizeof(pixel_t) * img->header.biWidth * img->header.biHeight);
    string_length = img->header.biWidth * sizeof(pixel_t);
    trash_len = (4 - (string_length % 4) )%4;
    
    for(i = 0; i < img->header.biHeight; i++)
    {
        if(fread(img->data + (img->header.biWidth * i), string_length, 1, in) != 1) 
        	return READ_INVALID_BITS;

        fseek(in, trash_len, SEEK_CUR);
    }

	return READ_OK;
}

write_error_code_t to_bmp_image(FILE* out, image_t* const img)
{
	int i, trash_len;
	size_t string_length = img->header.biWidth * sizeof(pixel_t);

    fseek(out, 0, SEEK_SET);

 	if(fwrite(&img->header, sizeof(bmp_header_t), 1, out) != 1)
		return WRITE_ERROR;

    trash_len = (4-(string_length % 4))%4;
    
    for(i = 0; i < img->header.biHeight; i++)
	{
		if(fwrite(img->data + (img->header.biWidth * i), string_length, 1, out) != 1)
            return WRITE_ERROR;

        fseek(out, trash_len, SEEK_CUR);
    }

	return WRITE_OK;
}

image_t rotate_right(image_t const source_image)
{
	int i, j;
	pixel_t* temp;
	
	image_t rotated_image = source_image;
	rotated_image.header.biWidth = source_image.header.biHeight;
	rotated_image.header.biHeight = source_image.header.biWidth;
	rotated_image.data = malloc(sizeof(pixel_t) * source_image.header.biHeight * source_image.header.biWidth);

	temp = rotated_image.data;
	
	for(i = source_image.header.biWidth - 1; i >= 0; i--)
	{
		for(j = 0; j < source_image.header.biHeight; j++)
		{
			*(temp++) = *(source_image.data + (j * source_image.header.biWidth + i));
		}
	}

	return rotated_image;
}

image_t rotate_left(image_t const source_image)
{
	int i, j;
	pixel_t* temp;
	
	image_t rotated_image = source_image;
	rotated_image.header.biWidth = source_image.header.biHeight;
	rotated_image.header.biHeight = source_image.header.biWidth;
	rotated_image.data = malloc(sizeof(pixel_t) * source_image.header.biHeight * source_image.header.biWidth);

	temp = rotated_image.data;

	for(i = 0; i < source_image.header.biWidth; i++)
	{
		for(j = source_image.header.biHeight - 1; j >= 0; j--)
		{
			*(temp++) = *(source_image.data + (j * source_image.header.biWidth + i));
		}
	}

	return rotated_image;
}
