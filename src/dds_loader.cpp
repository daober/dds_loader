#include "dds_loader.h"


int load_dds_from_file(char* filepath, DDS_HEADER** ddsfile) {
	//err = -1 -> file not found
	//err = -2 -> size of file is 0
	int err = 0x0000;

	//load texture from filepath
	FILE* p_file;

	p_file = fopen(filepath, "rb");
	if (!p_file) {
		err = -1;
		printf("ERROR::dds_loader -> file not found | ERRORCODE: %#08X", err);

	} else {
		fseek(p_file, 0L, SEEK_END);
		int sz = ftell(p_file);
		if (sz > 0) {
			//actual loading begins here
			err = fill_dds_info(p_file, ddsfile);
		} else {
			err = -2;
			printf("ERROR::dds_loader -> file found but file is empty | ERRORCODE: %#08X", err);
		}
		fclose(p_file);
	}
	return err;
}


int fill_dds_info(FILE* p_file, DDS_HEADER** ddsfile) {

	return 0;
}