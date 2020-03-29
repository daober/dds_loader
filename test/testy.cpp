#include "gtest/gtest.h"

#include "dds_loader.h"

int dubi(int a, int b) {
	return a + b;
}


TEST(testi, test0) {
	//int a = dubi(1, 4);
	int a = addi(1, 4);
	EXPECT_EQ(a, 5);
}