#include "gtest/gtest.h"

void foo(const std::vector<std::vector<int>>& mat)
{
    std::vector<int> ofs;
    int i, j;
    for (i = 0; i < mat.size(); i++)
        for (j = 0; j < mat[i].size(); j++)
            ofs.push_back(i + j);
}

TEST(Imgproc_Sobel, t1){}
TEST(Imgproc_Sobel, t2){}
TEST(Imgproc_Sobel, t3){}
TEST(Imgproc_Sobel, t4){}
TEST(Imgproc_Sobel, t5){}
