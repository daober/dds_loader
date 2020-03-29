#include "gtest/gtest.h"

#include "dds_loader.h"
#include <stdlib.h>



TEST(TestFilePath, TestInvalidFilePath) {
	DDS_HEADER* ddsfile = (DDS_HEADER*)malloc(sizeof(DDS_HEADER));
	int result = load_dds_from_file("../../assets/invalid_path.dds", &ddsfile);

	EXPECT_EQ(result, -1);

	free(ddsfile);
}

TEST(TestFilePath, TestSizeIsNull) {
	DDS_HEADER* ddsfile = (DDS_HEADER*)malloc(sizeof(DDS_HEADER));
	int result = load_dds_from_file("../../assets/zero_size.dds", &ddsfile);

	EXPECT_EQ(result, -2);

	free(ddsfile);
}



TEST(TestLoading, FileMatches) {
	DDS_HEADER* ddsfile = (DDS_HEADER*)malloc(sizeof( DDS_HEADER));
	int result = load_dds_from_file("../../assets/Shopsign_Bakery_BaseColor.dds", &ddsfile);

	EXPECT_EQ(result, 0);

	free(ddsfile);
}