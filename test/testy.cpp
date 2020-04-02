#include "gtest/gtest.h"

#include "dds_loader.h"
#include <stdlib.h>



TEST(TestFilePath, TestInvalidFilePath) {
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../../assets/invalid_path.dds", &dds_texture);

	EXPECT_EQ(result, -1);

	free(dds_texture);
}

TEST(TestFilePath, TestSizeIsNull) {
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../../assets/zero_size.dds", &dds_texture);

	EXPECT_EQ(result, -2);

	free(dds_texture);
}

TEST(TestLoading, FileWithDepth) {
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../../assets/MASTER_Room_Interior_BaseColor.dds", &dds_texture);

	EXPECT_EQ(result, 0);

	free(dds_texture);
}

TEST(TestLoading, FileMatches) {
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../../assets/Shopsign_Bakery_BaseColor.dds", &dds_texture);

	EXPECT_EQ(result, 0);

	free(dds_texture);
}


