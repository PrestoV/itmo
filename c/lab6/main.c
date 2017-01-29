#include <stdlib.h>
#include "imagelib.h"

int main(int argc, char **argv)
{
	FILE* file_source_image, *file_new_image;
	image_t source_image, new_image;
	int error_code;

	if(argc < 3 || argc > 4)
	{
		printf("Invalid arguments! \nArguments type: [lr] *image_name*.bmp OR *image_name*.bmp *rotated_image_name*.bmp\n");
		return 1;
	}

	if(argv[1][0] != 'l' && argv[1][0] != 'r')
	{
		printf("Incorrect rotation mode!\n");
		return 1;
	}
	
	file_source_image = (argc == 3) ? fopen(argv[2], "r+b") : fopen(argv[2], "rb");
	if(file_source_image == NULL)
	{
		printf("Сan't open the file: %s\n", argv[2]);
		return 4;
	}

	if(argc == 4)
	{
		file_new_image = fopen(argv[3], "wb");
		if(file_new_image == NULL)
		{
			printf("Сan't write to file: %s\n", argv[3]);
			return 4;
		}
	}

	error_code = from_bmp_image(file_source_image, &source_image);
	if(error_code != 0)
	{
		printf("Error reading from file: %s\n", argv[2]);
		return error_code;
	}

	new_image = (argv[1][0] == 'r') ? rotate_right(source_image) : rotate_left(source_image);

    error_code =  to_bmp_image(  ((argc == 3) ? file_source_image : file_new_image) , &new_image);

	if(error_code != 0)
	{
		printf("Error writing to file: %s\n", (argc == 3) ? argv[2] : argv[3]);
		return error_code;
	}

    fclose(file_new_image);

	return 0;
}
