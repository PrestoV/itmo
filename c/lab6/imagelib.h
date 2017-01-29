#include <stdio.h>
#include "image.h"

#ifndef _IMAGELIB_H_
#define _IMAGELIB_H_

read_error_code_t from_bmp_image(FILE* in, image_t* const img);
write_error_code_t to_bmp_image(FILE* out, image_t* const img);
image_t rotate_right(image_t const source_image);
image_t rotate_left(image_t const source_image);

#endif
