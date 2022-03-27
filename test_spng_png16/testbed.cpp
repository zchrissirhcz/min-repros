#include "spng.h"
#include <cstddef>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <string>

#include <opencv2/opencv.hpp>

#include "autotimer.hpp"

using namespace cv;

#include "png.h"
#include "zlib.h"

int save_image_spng(const std::string& image_path, const cv::Mat& src, const std::vector<int>& params = {})
{
    int depth = src.depth();
    if( depth != CV_8U && depth != CV_16U )
        return false;

    bool m_buf = false;

    cv::Size size = src.size();
    const uint32_t height = size.height;
    const uint32_t width = size.width;
    const size_t image_size = height * width * src.elemSize();
    void* image = src.data;
    size_t png_size;
    int ret = 0;
    FILE* volatile f = nullptr;
    const int channels = src.channels();

    /* Creating an encoder context requires a flag */
    spng_ctx *ctx = spng_ctx_new(SPNG_CTX_ENCODER);


    if (m_buf) 
    {
        // png_set_write_fn(png_ptr, this,
        //     (png_rw_ptr)writeDataToBuf, (png_flush_ptr)flushBuf);
        
        //spng_set_png_stream(ctx, writeDataToBuf, flushBuf);
    }
    else
    {
        f = fopen(image_path.c_str(), "wb");
        if (f)
            spng_set_png_file(ctx, f);
    }

    /* Encode to internal buffer managed by the library */
    spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);

    /* Specify image dimensions, PNG format */
    struct spng_ihdr ihdr;
    ihdr.height = height;
    ihdr.width = width;
    ihdr.bit_depth = src.elemSize1() * 8;
    ihdr.color_type = channels == 1 ? SPNG_COLOR_TYPE_GRAYSCALE : channels == 3 ? SPNG_COLOR_TYPE_TRUECOLOR : SPNG_COLOR_TYPE_TRUECOLOR_ALPHA;
    ihdr.compression_method = 0;
    ihdr.filter_method = 0;
    ihdr.interlace_method = 0;

    //-------------------------
    int compression_level = -1;
    int compression_strategy = IMWRITE_PNG_STRATEGY_RLE; // Default strategy
    bool isBilevel = false; // biBilevel is for png_set_packing(), which not supported by libspng now(2022.3.27)

    for( size_t i = 0; i < params.size(); i += 2 )
    {
        if( params[i] == IMWRITE_PNG_COMPRESSION )
        {
            compression_strategy = IMWRITE_PNG_STRATEGY_DEFAULT; // Default strategy
            compression_level = params[i+1];
            compression_level = MIN(MAX(compression_level, 0), Z_BEST_COMPRESSION);
        }
        if( params[i] == IMWRITE_PNG_STRATEGY )
        {
            compression_strategy = params[i+1];
            compression_strategy = MIN(MAX(compression_strategy, 0), Z_FIXED);
        }
        if( params[i] == IMWRITE_PNG_BILEVEL )
        {
            isBilevel = params[i+1] != 0;
        }
    }

    if (m_buf || f)
    {
        if( compression_level >= 0 )
        {
            //png_set_compression_level( png_ptr, compression_level );
            spng_set_option(ctx, SPNG_IMG_COMPRESSION_LEVEL, compression_level);
        }
        else
        {
            // tune parameters for speed
            // (see http://wiki.linuxquestions.org/wiki/Libpng)
            // png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, PNG_FILTER_SUB);
            spng_set_option(ctx, SPNG_FILTER_CHOICE, SPNG_FILTER_CHOICE_SUB);
            //png_set_compression_level(png_ptr, Z_BEST_SPEED);
            spng_set_option(ctx, SPNG_IMG_COMPRESSION_LEVEL, Z_BEST_SPEED);
        }
        //png_set_compression_strategy(png_ptr, compression_strategy);
        spng_set_option(ctx, SPNG_IMG_COMPRESSION_STRATEGY, compression_strategy);
        //-------------------------

        /* Image will be encoded according to ihdr.color_type, .bit_depth */
        ret = spng_set_ihdr(ctx, &ihdr);
        if (ret != 0)
        {
            printf("spng_set_ihdr() error: %s\n", spng_strerror(ret));
        }

        // png_set_bgr(); // There is no replacement of `png_set_bgr()` in libspng. we have to manually implement it
        if (channels == 3)
        {
            cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
        }
        else if (channels == 4)
        {
            cv::cvtColor(src, src, cv::COLOR_BGRA2RGBA);
        }

        /* SPNG_FMT_PNG is a special value that matches the format in ihdr,
        SPNG_ENCODE_FINALIZE will finalize the PNG with the end-of-file marker */
        spng_format fmt = SPNG_FMT_PNG;
        ret = spng_encode_image(ctx, image, image_size, fmt, SPNG_ENCODE_FINALIZE);
        if (ret != 0) {
            printf("spng_encode_image() error: %s\n", spng_strerror(ret));
        }
    }

    /* Free context memory */
    spng_ctx_free(ctx);

    if(f) fclose(f);

    return 0;
}



int save_image_libpng(const std::string& filename, const cv::Mat& img, const std::vector<int>& params = {})
{
    bool m_buf = false;

    png_structp png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
    png_infop info_ptr = 0;
    FILE * volatile f = 0;
    int y, width = img.cols, height = img.rows;
    int depth = img.depth(), channels = img.channels();
    volatile bool result = false;
    cv::AutoBuffer<uchar*> buffer;

    if( depth != CV_8U && depth != CV_16U )
        return false;

    if( png_ptr )
    {
        info_ptr = png_create_info_struct( png_ptr );

        if( info_ptr )
        {
            if( setjmp( png_jmpbuf ( png_ptr ) ) == 0 )
            {
                if( m_buf )
                {
                    // png_set_write_fn(png_ptr, this,
                    //     (png_rw_ptr)writeDataToBuf, (png_flush_ptr)flushBuf);
                }
                else
                {
                    f = fopen( filename.c_str(), "wb" );
                    if( f )
                        png_init_io( png_ptr, (png_FILE_p)f );
                }

                int compression_level = -1; // Invalid value to allow setting 0-9 as valid
                int compression_strategy = IMWRITE_PNG_STRATEGY_RLE; // Default strategy
                bool isBilevel = false;

                for( size_t i = 0; i < params.size(); i += 2 )
                {
                    if( params[i] == IMWRITE_PNG_COMPRESSION )
                    {
                        compression_strategy = IMWRITE_PNG_STRATEGY_DEFAULT; // Default strategy
                        compression_level = params[i+1];
                        compression_level = MIN(MAX(compression_level, 0), Z_BEST_COMPRESSION);
                    }
                    if( params[i] == IMWRITE_PNG_STRATEGY )
                    {
                        compression_strategy = params[i+1];
                        compression_strategy = MIN(MAX(compression_strategy, 0), Z_FIXED);
                    }
                    if( params[i] == IMWRITE_PNG_BILEVEL )
                    {
                        isBilevel = params[i+1] != 0;
                    }
                }

                if( m_buf || f )
                {
                    if( compression_level >= 0 )
                    {
                        png_set_compression_level( png_ptr, compression_level );
                    }
                    else
                    {
                        // tune parameters for speed
                        // (see http://wiki.linuxquestions.org/wiki/Libpng)
                        png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, PNG_FILTER_SUB);
                        png_set_compression_level(png_ptr, Z_BEST_SPEED);
                    }
                    png_set_compression_strategy(png_ptr, compression_strategy);

                    png_set_IHDR( png_ptr, info_ptr, width, height, depth == CV_8U ? isBilevel?1:8 : 16,
                        channels == 1 ? PNG_COLOR_TYPE_GRAY :
                        channels == 3 ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGBA,
                        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                        PNG_FILTER_TYPE_DEFAULT );

                    png_write_info( png_ptr, info_ptr );

                    if (isBilevel)
                        png_set_packing(png_ptr);

                    png_set_bgr( png_ptr );
                    // if( !isBigEndian() )
                    //     png_set_swap( png_ptr );
                    #if !TV_IS_BIG_ENDIAN
                        png_set_swap( png_ptr );
                    #endif // TV_IS_BIG_ENDIAN

                    buffer.allocate(height);
                    for( y = 0; y < height; y++ )
                        buffer[y] = img.data + y*img.step;

                    png_write_image( png_ptr, buffer.data() );
                    png_write_end( png_ptr, info_ptr );

                    result = true;
                }
            }
        }
    }

    png_destroy_write_struct( &png_ptr, &info_ptr );
    if(f) fclose( (FILE*)f );

    return result;
}

static void normalize_16UC1_to_8UC1(const cv::Mat& src, cv::Mat& normed)
{
    double minval;
    double maxval;
    cv::minMaxLoc(src, &minval, &maxval);

    normed.create(src.size(), CV_8UC1);
    int height = src.size().height;
    int width  = src.size().width;
    uint16_t minval_u16 = (uint16_t)minval;
    double diff = maxval - minval;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            normed.ptr<uchar>(i, j)[0] =  255 * ( (src.ptr<uint16_t>(i, j)[0] - minval_u16) / diff );
        }
    }
}

void show_png16(const std::string& image_path)
{
    cv::Mat image = cv::imread(image_path, cv::IMREAD_UNCHANGED);
    cv::Mat normed_image;
    normalize_16UC1_to_8UC1(image, normed_image);
    cv::imshow("image", image);
    cv::imshow("normed_image", normed_image);
    cv::waitKey(0);
}

int main()
{
    for (std::string image_path : {"1920x1080.png", "depth_image.png"})
    {
        cv::Mat src = cv::imread(image_path, cv::IMREAD_UNCHANGED);
        fprintf(stderr, "encoding %s, height=%d, width=%d, channels=%d, pixelsize=%lu\n",
            image_path.c_str(), src.rows, src.cols, src.channels(), src.elemSize()
        );

        {
            AutoTimer timer("libpng");
            save_image_libpng("result_libpng.png", src);
        }

        {
            AutoTimer timer("spng");
            save_image_spng("result_libspng.png", src);
        }
    }

    return 0;
}