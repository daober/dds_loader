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


/*======================================================================*/
/*===== VULKAN DEFINES (should also work with OPENGL APPLICATIONS) =====*/


/*======================================================================*/



enum { TYPE_TEXTURE_NONE = -1, TYPE_TEXTURE_FLAT, TYPE_TEXTURE_3D, TYPE_TEXTURE_CUBEMAP };


int is_compressed_texture(int format) {
	return ( format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || (format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT) || (format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) );
}

int flip_texture(DDS_TEXTURE** texture_in)
{
	return 0;
}


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

	int type = 0;
	int format = 0;
	int channels = 0;

	//default format
	type = TYPE_TEXTURE_FLAT;

	if (ddsh->dwCaps2 & DDSF_CUBEMAP) {
		type = TYPE_TEXTURE_CUBEMAP;
	}
	else if ((ddsh->dwCaps2 & DDSF_VOLUME) && (ddsh->dwDepth > 0)) {
		type = TYPE_TEXTURE_3D;
	}
	else {
		type = TYPE_TEXTURE_FLAT;
	}

	// figure out what image format it is
	if ((ddsh->ddspf.dwFlags & DDSF_FOURCC)) {
		switch (ddsh->ddspf.dwFourCC) {
			case FOURCC_DXT1:
				format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				channels = 3;
				break;
			case FOURCC_DXT3:
				format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				channels = 4;
				break;
			case FOURCC_DXT5:
				format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				channels = 4;
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
		channels = 4;
	} else if (ddsh->ddspf.dwRGBBitCount == 32 &&
		ddsh->ddspf.dwRBitMask == 0x000000FF &&
		ddsh->ddspf.dwGBitMask == 0x0000FF00 &&
		ddsh->ddspf.dwBBitMask == 0x00FF0000 &&
		ddsh->ddspf.dwABitMask == 0xFF000000) {
		format = GL_RGBA;
		channels = 4;
	} else if (ddsh->ddspf.dwRGBBitCount == 24 &&
		ddsh->ddspf.dwRBitMask == 0x000000FF &&
		ddsh->ddspf.dwGBitMask == 0x0000FF00 &&
		ddsh->ddspf.dwBBitMask == 0x00FF0000) {
		format = GL_RGB;
		channels = 3;
	} else if (ddsh->ddspf.dwRGBBitCount == 24 &&
		ddsh->ddspf.dwRBitMask == 0x00FF0000 &&
		ddsh->ddspf.dwGBitMask == 0x0000FF00 &&
		ddsh->ddspf.dwBBitMask == 0x000000FF) {
		format = GL_BGR_EXT;
		channels = 3;
	} else if (ddsh->ddspf.dwRGBBitCount == 8) {
		format = GL_LUMINANCE;
		channels = 1;
	} else {
		err = -4;
		printf("ERROR::dds_loader -> unknown texture format | ERRORCODE: %#08X\n", err);
	}

	unsigned int num_mipmap = ddsh->dwMipMapCount ? ddsh->dwMipMapCount : 0;
	unsigned int picture_size = 0;
	unsigned int width, height, depth = 0;
	width = ddsh->dwWidth;
	height = ddsh->dwHeight;
	depth = ddsh->dwDepth == 0 ? 1 : ddsh->dwDepth;

	if (is_compressed_texture(format)) {
		picture_size = ((width + 3) / 4) * ((height + 3) / 4) * (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16);
	} else {
		picture_size = width * height * channels;
	}

	assert(picture_size != 0);

	unsigned char* pixels = (unsigned char*)malloc(sizeof( unsigned char ) * picture_size);
	memset(pixels, 0, sizeof(unsigned char) * picture_size);

	memcpy(pixels, buffer + 4 + sizeof(DDS_HEADER), sizeof(unsigned char) * picture_size);

	//load all mipmaps
	if (num_mipmap != 0) {
		num_mipmap--;		//excludes the 'main' texture
	}

	//fill returned structure
	(*texture_in)->channels = channels;
	(*texture_in)->sz = picture_size;
	(*texture_in)->depth = depth;
	(*texture_in)->width = width;
	(*texture_in)->height = height;
	(*texture_in)->format = format;
	(*texture_in)->mipmap_count = num_mipmap;

	(*texture_in)->pixels = (unsigned char*)malloc(sizeof(unsigned char) * picture_size);
	memset((*texture_in)->pixels, 0, sizeof(unsigned char) * picture_size);
	memcpy((*texture_in)->pixels, pixels, sizeof(unsigned char) * picture_size);

	//initial shrink of texture size (for mipmaps)
	unsigned int mip_w = width >> 1;
	unsigned int mip_h = height >> 1;
	unsigned int mip_d = depth >> 1 ? depth >> 1 : 1;
	
	unsigned int offset = 0;

	//load all mipmaps
	for (unsigned int i = 0; i < num_mipmap && (mip_w || mip_h); i++) {

		unsigned int mip_picture_size = 0;
		if (is_compressed_texture(format)) {
			mip_picture_size = ((mip_w + 3) / 4) * ((mip_h + 3) / 4) * (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16);
		}
		else {
			mip_picture_size = mip_w * mip_h * channels;
		}

		//allocate mipmap structure array
		(*texture_in)->mipmaps = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));

		if ((*texture_in)->mipmaps == NULL) {
			return -5;
		}

		(*texture_in)->mipmaps->mipmaps = NULL;

		//fill mipmap structure
		memset(&(*texture_in)->mipmaps->channels, 0, sizeof(int));
		memcpy(&(*texture_in)->mipmaps->channels, &channels, sizeof(int));

		memset(&(*texture_in)->mipmaps->sz, 0, sizeof(unsigned int));
		memcpy(&(*texture_in)->mipmaps->sz, &mip_picture_size, sizeof(unsigned int));

		memset(&(*texture_in)->mipmaps->depth, 0, sizeof(unsigned int));
		memcpy(&(*texture_in)->mipmaps->depth, &mip_d, sizeof(unsigned int));

		memset(&(*texture_in)->mipmaps->width, 0, sizeof(unsigned int));
		memcpy(&(*texture_in)->mipmaps->width, &mip_w, sizeof(unsigned int));

		memset(&(*texture_in)->mipmaps->height, 0, sizeof(unsigned int));
		memcpy(&(*texture_in)->mipmaps->height, &mip_h, sizeof(unsigned int));

		memset(&(*texture_in)->mipmaps->format, 0, sizeof(unsigned int));
		memcpy(&(*texture_in)->mipmaps->format, &format, sizeof(unsigned int));

		memset(&(*texture_in)->mipmaps->mipmap_count, 0, sizeof(unsigned int));
		memcpy(&(*texture_in)->mipmaps->mipmap_count, &i, sizeof(unsigned int));

		(*texture_in)->mipmaps->pixels = (unsigned char*)malloc(sizeof(unsigned char) * mip_picture_size);

		memset((*texture_in)->mipmaps->pixels, 0, sizeof(unsigned char) * mip_picture_size);
		memcpy((*texture_in)->mipmaps->pixels, pixels, sizeof(unsigned char) * mip_picture_size);


		//TODO: need to calculate the exact size to next mipmap block
		(*texture_in)->mipmaps->pixels += mip_picture_size;
		(*texture_in)->mipmaps += sizeof(DDS_TEXTURE) + sizeof(mip_picture_size) + sizeof(channels) + sizeof(unsigned char) + sizeof(mip_d) + sizeof(mip_w) + sizeof(mip_h) + sizeof(format) + sizeof(i);


		/*
		
		typedef struct DDS_TEXTURE {
	unsigned int			width;
	unsigned int			height;
	unsigned int			depth;
	unsigned int			sz;
	unsigned int			channels;
	unsigned int			format;
	unsigned int			mipmap_count;		//mipmap_count is index in mipmaps

	struct DDS_TEXTURE*		mipmaps;

	unsigned char*			pixels;
} DDS_TEXTURE; */
		//shrink again
		mip_w = mip_w >> 1;
		mip_h = mip_h >> 1;
		mip_d = mip_d >> 1 ? mip_d >> 1 : 1;

		//fill the next inner mipmap structure
		//offset += mip_picture_size;

		offset += mip_picture_size;
	}

	free(pixels);

	free(buffer);
	free(ddsh);

	return err;
}