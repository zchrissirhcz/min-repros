#include <gtest/gtest.h>

typedef unsigned short ushort;

template<typename T>
struct CvtVectorTest : public ::testing::Test
{
public:
    void testAll()
    {
        std::vector<T> v;
        for (int i = 0; i < 255; i++)
            v.push_back((T) i);
    }
};

typedef ::testing::Types<ushort, short, int, float, double> CustomedImplementations;
TYPED_TEST_SUITE(CvtVectorTest, CustomedImplementations);

TYPED_TEST(CvtVectorTest, All)
{
    this->testAll();
}
