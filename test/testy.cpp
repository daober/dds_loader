#include "gtest/gtest.h"

#include "dds_loader.h"




TEST(TestFilePath, TestInvalidFilePath) {
	DDS_HEADER* ddsfile = new DDS_HEADER();
	int result = load_dds_from_file("../../assets/invalid_path.dds", &ddsfile);

	EXPECT_EQ(result, -1);

	delete ddsfile;
}

TEST(TestFilePath, TestSizeIsNull) {
	DDS_HEADER* ddsfile = new DDS_HEADER();
	int result = load_dds_from_file("../../assets/zero_size.dds", &ddsfile);

	EXPECT_EQ(result, -2);

	delete ddsfile;
}