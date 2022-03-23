#include <gtest/gtest.h>
#include <thread>
#include "my.hpp"

void my_test_parallel_MultipleThreads()
{
    std::thread ts[2];

    int num_threads = 1;
    fprintf(stderr, "num_threads = %d\n", num_threads);
    ts[0] = std::thread([num_threads] {
        const int N = 2;
        int count = 0;
        my::setNumThreads(num_threads);
        my::parallel_for(0, N, [&count](int) {

        });
    });

    num_threads = 2;
    fprintf(stderr, "num_threads = %d\n", num_threads);
    ts[1] = std::thread([num_threads] {
        const int N = 2;
        int count = 0;
        my::setNumThreads(num_threads);
        my::parallel_for(0, N, [&count](int) {

        });
    });

    ts[0].join();
    ts[1].join();
}

int main()
{
    my_test_parallel_MultipleThreads();
    return 0;
}