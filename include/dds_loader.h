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
	unsigned int           dwSize;
	unsigned int           dwFlags;
	unsigned int           dwHeight;
	unsigned int           dwWidth;
	unsigned int           dwPitchOrLinearSize;
	unsigned int           dwDepth;
	unsigned int           dwMipMapCount;
	unsigned int           dwReserved1[11];
	DDS_PIXELFORMAT		   ddspf;
	unsigned int           dwCaps;
	unsigned int           dwCaps2;
	unsigned int           dwCaps3;
	unsigned int           dwCaps4;
	unsigned int           dwReserved2;
} DDS_HEADER;


typedef struct {
	unsigned int			width;
	unsigned int			height;
	unsigned int			depth;
	unsigned int			sz;
	unsigned int			channels;
	unsigned int			format;
	//no mipmap count
	unsigned char*			pixels;
} MIPMAP_TEXTURE;


typedef struct {
	unsigned int			width;
	unsigned int			height;
	unsigned int			depth;
	unsigned int			sz;
	unsigned int			channels;
	unsigned int			format;
	unsigned int			mipmap_count;

	MIPMAP_TEXTURE*			mipmaps;

	unsigned char*			pixels;
} DDS_TEXTURE;



int load_dds_from_file(char* filepath, DDS_TEXTURE** texture_in, const bool flip = false);

int fill_dds_info(FILE* p_file, DDS_TEXTURE** texture_in, const int size, const bool flip = false);

int is_compressed_texture(int format);

int flip_texture(DDS_TEXTURE** texture_in);

#endif