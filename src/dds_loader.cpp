#include "dds_loader.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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


	//size_t fread(void* ptr, size_t size_of_elements, size_t number_of_elements, FILE * a_file);
	free(buffer);
	free(ddsh);

	return err;
}