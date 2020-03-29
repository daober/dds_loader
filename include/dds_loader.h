#ifndef _DDS_LOADER_H_
#define _DDS_LOADER_H_

#include <stdio.h>

typedef struct {
	unsigned int			dwSize;
	unsigned int			dwFlags;
	unsigned int			dwFourCC;
	unsigned int			dwRGBBitCount;
	unsigned int			dwRBitMask;
	unsigned int			dwGBitMask;
	unsigned int			dwBBitMask;
	unsigned int			dwABitMask;
} DDS_PIXELFORMAT;


typedef struct {
	unsigned int            dwSize;
	unsigned int            dwFlags;
	unsigned int            dwHeight;
	unsigned int            dwWidth;
	unsigned int            dwPitchOrLinearSize;
	unsigned int            dwDepth;
	unsigned int            dwMipMapCount;
	unsigned int            dwReserved1[11];
	DDS_PIXELFORMAT			ddspf;
	unsigned int            dwCaps;
	unsigned int            dwCaps2;
	unsigned int            dwCaps3;
	unsigned int            dwCaps4;
	unsigned int            dwReserved2;
} DDS_HEADER;


int load_dds_from_file(char* filepath, DDS_HEADER** ddsfile, const bool flip = false);

int fill_dds_info(FILE* p_file, DDS_HEADER** ddsfile, const int size, const bool flip = false);

#endif