#include "warpaffine.hpp"
#include <opencv2/opencv.hpp>
#include "autotimer.hpp"

int main()
{
    constexpr int width = 160;
    constexpr int height = 160;

#if __linux__
    std::string image_path = "/home/zz/data/tallgirl.jpg";
#else // assume android
    std::string image_path = "/data/local/tmp/tallgirl.jpg";
#endif

    cv::Mat image = cv::imread(image_path);
    cv::cvtColor(image, image, cv::COLOR_BGR2BGRA);
    unsigned char* src = image.data;
    int srcw = image.cols;
    int srch = image.rows;
    int srcstride = image.step;

    int w = srcw / 2;
    int h = srch / 2;
    int stride = srcstride / 2;

    unsigned int v = 0;
    constexpr float tm[6] = {
        -0.00673565036, 0.146258384, 4.34562492,
        -0.146258384, -0.00673565036, 162.753372
    };

    cv::Size result_size;
    result_size.height = h;
    result_size.width = w;
    cv::Mat result(result_size, CV_8UC4);
    unsigned char* dst = result.data;

    {
        AutoTimer timer("warpaffine_bilinear_c4");
        warpaffine_bilinear_c4(src, srcw, srch, srcstride, dst, w, h, stride, tm);
    }

    return 0;
}