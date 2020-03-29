#include "gtest/gtest.h"

#include "dds_loader.h"




TEST(testi, test0) {
	int a = addi(1, 4);
	EXPECT_EQ(a, 5);
}