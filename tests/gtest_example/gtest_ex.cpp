#include <gtest/gtest.h>
int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

// 测试套件：CalcTest，测试加法
TEST(CalcTest, AddFunc) {
    EXPECT_EQ(add(1, 2), 3);   // 1+2=3 
    EXPECT_EQ(add(0, 0), 0);   // 0+0=0 
    EXPECT_EQ(add(-1, 1), 0);  // -1+1=0 
}

// 测试套件：CalcTest，测试减法
TEST(CalcTest, SubFunc) {
    EXPECT_EQ(sub(5, 3), 2);   // 5-3=2 
    EXPECT_EQ(sub(3, 5), -2);  // 3-5=-2 
    ASSERT_EQ(sub(0, 0), 0);   // 0-0=0 
}
