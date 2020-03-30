#include "dds_loader.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

// dwCaps1 flags
#define DDSF_COMPLEX		0x00000008
#define DDSF_TEXTURE		0x00001000
#define DDSF_MIPMAP			0x00400000

// dwCaps2 flags
//--- missing ---

// pixel format flags
#define DDSF_ALPHAPIXELS	0x00000001
#define DDSF_FOURCC			0x00000004
#define DDSF_RGB			0x00000040
#define DDSF_RGBA			0x00000041





int load_dds_from_file(char* filepath, DDS_HEADER** ddsfile, const bool flip) {
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
			err = fill_dds_info(p_file, ddsfile, sz, flip);
		} else {
			err = -2;
			printf("ERROR::dds_loader -> file found but file is empty | ERRORCODE: %#08X\n", err);
		}
		fclose(p_file);
	}
	return err;
}


int fill_dds_info(FILE* p_file, DDS_HEADER** ddsfile, const int size, const bool flip) {

	int err = 0;
	char* buffer = (char*)malloc(sizeof(char)*size);
	fread(buffer, 1, size, p_file);
	
	//check if file reading into buffer was successful
	assert(buffer != 0);

	if (strncmp(buffer, "DDS ", 4) != 0) {
		err = -3;
		printf("ERROR::dds_loader -> no valid dds file given | ERRORCODE: %#08X\n", err);
	}

	//read and fill dds header
	DDS_HEADER* ddsh = (DDS_HEADER*)malloc(sizeof(DDS_HEADER));
	memcpy(ddsh, buffer, sizeof(DDS_HEADER));

	//caps 0x1
	//width 0x2
	//height 0x4
	if (ddsh->dwFlags & 0x2) {
		printf("valid width");
	}


	//size_t fread(void* ptr, size_t size_of_elements, size_t number_of_elements, FILE * a_file);
	free(buffer);
	free(ddsh);

	return err;
}