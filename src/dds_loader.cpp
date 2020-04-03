#include "dds_loader.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>



//surface description flags
#define DDSF_CAPS				0x00000001
#define DDSF_HEIGHT				0x00000002
#define DDSF_WIDTH				0x00000004
#define DDSF_PITCH				0x00000008
#define DDSF_PIXELFORMAT		0x00001000
#define DDSF_MIPMAPCOUNT		0x00020000
#define DDSF_LINEARSIZE			0x00080000
#define DDSF_DEPTH				0x00800000

// dwCaps1 flags
#define DDSF_COMPLEX			0x00000008
#define DDSF_TEXTURE			0x00001000
#define DDSF_MIPMAP				0x00400000

// dwCaps2 flags
#define DDSF_CUBEMAP			0x00000200
#define DDSF_CUBEMAP_POSITIVEX	0x00000400
#define DDSF_CUBEMAP_NEGATIVEX	0x00000800
#define DDSF_CUBEMAP_POSITIVEY	0x00001000
#define DDSF_CUBEMAP_NEGATIVEY	0x00002000
#define DDSF_CUBEMAP_POSITIVEZ	0x00004000
#define DDSF_CUBEMAP_NEGATIVEZ	0x00008000
#define DDSF_CUBEMAP_ALL_FACES	0x0000FC00
#define DDSF_VOLUME				0x00200000

// pixel format flags
#define DDSF_ALPHAPIXELS		0x00000001
#define DDSF_FOURCC				0x00000004
#define DDSF_RGB				0x00000040
#define DDSF_RGBA				0x00000041

// compressed texture types
#define FOURCC_DXT1				0x31545844 //(MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT3				0x33545844 //(MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT5				0x35545844 //(MAKEFOURCC('D','X','T','5'))

#define GL_BGR_EXT                                        0x80E0
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT                   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT                  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT                  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT                  0x83F3

#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LUMINANCE                      0x1909
#define GL_BGR_EXT                        0x80E0
#define GL_BGRA_EXT                       0x80E1

enum { TYPE_TEXTURE_NONE = -1, TYPE_TEXTURE_FLAT, TYPE_TEXTURE_3D, TYPE_TEXTURE_CUBEMAP };



int load_dds_from_file(char* filepath, DDS_TEXTURE** texture_in, const bool flip) {
	//err = -1 -> file not found
	//err = -2 -> size of file is 0
	int err = 0x0000;

	//load texture from filepath
	FILE* p_file;

	p_file = fopen(filepath, "rb");
	if (!p_file) {
		err = -1;
		printf("ERROR::dds_loader -> file not found | ERRORCODE: %#08X\n", err);

	} else {
		fseek(p_file, 0L, SEEK_END);
		int sz = ftell(p_file);
		//back to the beginning
		rewind(p_file);
		if (sz > 0) {
			//actual loading begins here
			err = fill_dds_info(p_file, texture_in, sz, flip);
		} else {
			err = -2;
			printf("ERROR::dds_loader -> file found but file is empty | ERRORCODE: %#08X\n", err);
		}
		fclose(p_file);
	}
	return err;
}


int fill_dds_info(FILE* p_file, DDS_TEXTURE** texture_in, const int size, const bool flip) {

	int err = 0;
	char* buffer = (char*)malloc(sizeof(char)*size);
	fread(buffer, size, 1, p_file);
	
	//check if file reading into buffer was successful
	assert(buffer != 0);

	if (strncmp(buffer, "DDS ", 4) != 0) {
		err = -3;
		printf("ERROR::dds_loader -> no valid dds file given | ERRORCODE: %#08X\n", err);
		return err;
	}

	//read and fill dds header
	DDS_HEADER* ddsh = (DDS_HEADER*)malloc(sizeof(DDS_HEADER));

	//memcpy + shift to ignore the magic "DDS " identifier
	memcpy(ddsh, buffer + 4, sizeof(DDS_HEADER));

	//assert is thrown because because offset isn't right
	assert(ddsh->dwSize == 124);

	//caps 0x1
	//width 0x2
	//height 0x4
	int type = 0;
	int format = 0;
	int components = 0;

	//default format
	type = TYPE_TEXTURE_FLAT;

	if (ddsh->dwCaps2 & DDSF_CUBEMAP) {
		type = TYPE_TEXTURE_CUBEMAP;
	}
	else if ((ddsh->dwCaps2 & DDSF_VOLUME) && (ddsh->dwDepth > 0)) {
		type = TYPE_TEXTURE_3D;
	}
	else {
		type = TYPE_TEXTURE_NONE;
	}

	// figure out what image format it is
	if ((ddsh->ddspf.dwFlags & DDSF_FOURCC)) {
		switch (ddsh->ddspf.dwFourCC) {
			case FOURCC_DXT1:
				format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				components = 3;
				break;
			case FOURCC_DXT3:
				format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				components = 4;
				break;
			case FOURCC_DXT5:
				format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				components = 4;
				break;
			default:
				err = -4;
				printf("ERROR::dds_loader -> unknown texture format | ERRORCODE: %#08X\n", err);
		}
	} else if (ddsh->ddspf.dwRGBBitCount == 32 &&
		ddsh->ddspf.dwRBitMask == 0x00FF0000 &&
		ddsh->ddspf.dwGBitMask == 0x0000FF00 &&
		ddsh->ddspf.dwBBitMask == 0x000000FF &&
		ddsh->ddspf.dwABitMask == 0xFF000000) {

		format = GL_BGRA_EXT;
		components = 4;
	} else if (ddsh->ddspf.dwRGBBitCount == 32 &&
		ddsh->ddspf.dwRBitMask == 0x000000FF &&
		ddsh->ddspf.dwGBitMask == 0x0000FF00 &&
		ddsh->ddspf.dwBBitMask == 0x00FF0000 &&
		ddsh->ddspf.dwABitMask == 0xFF000000) {
		format = GL_RGBA;
		components = 4;
	} else if (ddsh->ddspf.dwRGBBitCount == 24 &&
		ddsh->ddspf.dwRBitMask == 0x000000FF &&
		ddsh->ddspf.dwGBitMask == 0x0000FF00 &&
		ddsh->ddspf.dwBBitMask == 0x00FF0000) {
		format = GL_RGB;
		components = 3;
	} else if (ddsh->ddspf.dwRGBBitCount == 24 &&
		ddsh->ddspf.dwRBitMask == 0x00FF0000 &&
		ddsh->ddspf.dwGBitMask == 0x0000FF00 &&
		ddsh->ddspf.dwBBitMask == 0x000000FF) {
		format = GL_BGR_EXT;
		components = 3;
	} else if (ddsh->ddspf.dwRGBBitCount == 8) {
		format = GL_LUMINANCE;
		components = 1;
	} else {
		err = -4;
		printf("ERROR::dds_loader -> unknown texture format | ERRORCODE: %#08X\n", err);
	}

	unsigned int width, height, depth;
	width = ddsh->dwWidth;
	height = ddsh->dwHeight;
	depth = ddsh->dwDepth == 0 ? 1: ddsh->dwDepth;

	unsigned int picture_size = width * height * depth * 12;

	assert(picture_size != 0);
	
	
	//TODO: calculations are missing when texture is compressed
	unsigned char* pixels = (unsigned char*)malloc(sizeof( unsigned char ) * picture_size);
	memset(pixels, 0, picture_size);
	//copy pixels buffer to DDS_TEXTURE


	//here it fails
	memcpy(pixels, buffer + 4 + sizeof(DDS_HEADER), sizeof(unsigned char*) * picture_size);

	(*texture_in)->channels = components;
	(*texture_in)->depth = depth;
	(*texture_in)->sz = picture_size;
	(*texture_in)->width = width;
	(*texture_in)->height = height;
	memcpy((*texture_in)->pixels, pixels, sizeof(unsigned char) * picture_size);

	delete[] pixels;

	//size_t fread(void* ptr, size_t size_of_elements, size_t number_of_elements, FILE * a_file);
	free(buffer);
	free(ddsh);

	return err;
}