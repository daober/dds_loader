#include "gtest/gtest.h"
#include "dds_loader.h"


TEST(TestFilePath, TestInvalidFilePath) {
	const int do_flip = 0;
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../assets/invalid_path.dds", &dds_texture, do_flip);

	EXPECT_EQ(result, -1);

	free(dds_texture);
}

TEST(TestFilePath, TestSizeIsNull) {
	const int do_flip = 0;
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../assets/zero_size.dds", &dds_texture, do_flip);

	EXPECT_EQ(result, -2);

	free(dds_texture);
}


TEST(TestLoading, FileWithDepth) {
	const int do_flip = 0;
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../assets/MASTER_Room_Interior_BaseColor.dds", &dds_texture, do_flip);

	EXPECT_EQ(result, 0);

	free(dds_texture);
}

TEST(TestLoading, FileMatches) {
	const int do_flip = 0;
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../assets/Shopsign_Bakery_BaseColor.dds", &dds_texture, do_flip);

	EXPECT_EQ(result, 0);

	free(dds_texture);
}

TEST(TestLoading, MainPixelsFilled) {
	const int do_flip = 0;
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../assets/Shopsign_Bakery_BaseColor.dds", &dds_texture, do_flip);

	if(dds_texture->pixels && dds_texture->sz){
		EXPECT_GT(dds_texture->sz, 0);

		EXPECT_TRUE(dds_texture->pixels != NULL);
		EXPECT_TRUE(dds_texture->pixels != nullptr);
		EXPECT_TRUE(dds_texture->pixels != 0);
	}
	free(dds_texture);
}

TEST(TestLoading, MainStructureFilled) {
	const int do_flip = 0;
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../assets/Shopsign_Bakery_BaseColor.dds", &dds_texture, do_flip);

	if(dds_texture->pixels && dds_texture->sz){
		EXPECT_GT(dds_texture->width, 0);
		EXPECT_GT(dds_texture->height, 0);
		EXPECT_GT(dds_texture->depth, 0);
		EXPECT_GT(dds_texture->channels, 0);
		EXPECT_GT(dds_texture->format, 0);
		EXPECT_GT(dds_texture->sz, 0);
		EXPECT_GE(dds_texture->mipmap_count, 0);
	}
	free(dds_texture);
}

TEST(TestLoading, MipMappingStructureFilled) {
	const int do_flip = 0;
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../assets/Shopsign_Bakery_BaseColor.dds", &dds_texture, do_flip);

	if(dds_texture->mipmap_count > 0){
		EXPECT_GT(dds_texture->mipmap_count, 0);
		if (dds_texture->mipmaps) {
			EXPECT_GT(dds_texture->mipmaps->width, 0);
			EXPECT_GT(dds_texture->mipmaps->height, 0);
			EXPECT_GT(dds_texture->mipmaps->depth, 0);
			EXPECT_GT(dds_texture->mipmaps->channels, 0);
			EXPECT_GT(dds_texture->mipmaps->format, 0);
			EXPECT_GT(dds_texture->mipmaps->sz, 0);
			
			EXPECT_TRUE(dds_texture->mipmaps->pixels != NULL);
			EXPECT_TRUE(dds_texture->mipmaps->pixels != nullptr);
			EXPECT_TRUE(dds_texture->mipmaps->pixels != 0);
		}
	} else {
		//just let it fail
		EXPECT_EQ(dds_texture->mipmap_count, 1);
	}
	free(dds_texture);
}

TEST(TestLoading, MipMappingStructureInnerMipFilled) {
	const int do_flip = 0;
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../assets/Shopsign_Bakery_BaseColor.dds", &dds_texture, do_flip);

	if(dds_texture->mipmap_count > 0){
		EXPECT_GT(dds_texture->mipmap_count, 0);
		if (dds_texture->mipmaps->mipmaps) {
			EXPECT_GT(dds_texture->mipmaps->mipmaps->width, 0);
			EXPECT_GT(dds_texture->mipmaps->mipmaps->height, 0);
			EXPECT_GT(dds_texture->mipmaps->mipmaps->channels, 0);
			EXPECT_GT(dds_texture->mipmaps->mipmaps->sz, 0);
		}
	} else {
		//just let it fail
		EXPECT_GT(0, 1);
	}
	free(dds_texture);
}

TEST(TestLoading, FlippedTexture) {
	const int do_flip = 1;
	DDS_TEXTURE* dds_texture = (DDS_TEXTURE*)malloc(sizeof(DDS_TEXTURE));
	int result = load_dds_from_file("../assets/Shopsign_Bakery_BaseColor.dds", &dds_texture, do_flip);

	EXPECT_EQ(result, 0);

	if (dds_texture->pixels && dds_texture->sz) {
		EXPECT_GT(dds_texture->sz, 0);

		EXPECT_TRUE(dds_texture->pixels != NULL);
		EXPECT_TRUE(dds_texture->pixels != nullptr);
		EXPECT_TRUE(dds_texture->pixels != 0);
	}
	free(dds_texture);
}

