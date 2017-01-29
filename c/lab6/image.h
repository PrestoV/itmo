#include <stdint.h>
#include <stdio.h>

#ifndef _IMAGE_H_
#define _IMAGE_H_

#pragma pack(push,2)
typedef struct {
	uint16_t bftype;
    uint32_t bfilesize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompresion;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} bmp_header_t;
#pragma pack(pop)

typedef struct{
	char r, g, b;
} pixel_t;

typedef struct{
    bmp_header_t header;
    pixel_t *data;
}image_t;

typedef enum {
	READ_OK = 0,
	READ_INVALID_SIGNATURE,
	READ_INVALID_BITS,
	READ_INVALID_HEADER
} read_error_code_t;

typedef enum {
	WRITE_OK = 0,
	WRITE_ERROR
} write_error_code_t;

#endif
