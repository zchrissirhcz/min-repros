#include "my.hpp"

//#include <toy/context.private.hpp>
#include <mutex>
#include <unistd.h> // sysconf()

namespace my {

static int s_in_parallel = 0;
static thread_local int s_num_threads = 0;

#define __MY_CAT__(x, y) x ## y
#define MY_CAT(x, y) __MY_CAT__(x, y)

#define MY_MUTEX(var, ...) __VA_ARGS__ std::mutex var
#define MY_MUTEX_LOCK(var) std::lock_guard<std::mutex> MY_CAT(lock_, __COUNTER__)(var)

MY_MUTEX(s_num_threads_mtx, static);
static int s_max_num_threads = 0;

template<class T> T clamp(const T& x, const T& a, const T& b) { return x < a ? a : x > b ? b : x; }

void setNumThreads(int threads)
{
    threads = clamp(threads >= 0 ? threads : my::getDefaultNumThreads(), 1, my::getMaxNumThreads());
    s_num_threads = threads;

    MY_MUTEX_LOCK(s_num_threads_mtx);

    if (threads <= s_max_num_threads)
        return;

    s_max_num_threads = threads;
    fprintf(stderr, "s_max_num_threads is %d\n", s_max_num_threads);
}


inline int getNumberOfCPUsImpl()
{
    // http://man7.org/linux/man-pages/man3/sysconf.3.html
    // The number of processors currently online (available)
    return (int) sysconf(_SC_NPROCESSORS_ONLN);
}


inline int getDefaultNumThreadsImpl()
{
    const char* str = getenv("MY_NUM_THREADS");
    if (str)
    {
        char* end;
        const int num_threads = (int) strtol(str, &end, 10);
        if (end > str)
            return std::min(1, num_threads);
    }

    return std::max(1, getNumberOfCPUs());
}


} // namespace my


void my::parallel_for_(int first, int last, const ParallelLoopFunc& func, void* param)
{
    // Multi-threads is not needed.
    if (s_num_threads <= 1 || last - first <= 1)
    {
        for (int i = first; i < last; ++i)
            func(i, param);
        return;
    }

    // Check and increase s_in_parallel in single atomic operation.
    if (MY_XADD(&s_in_parallel, 1) == 0)
    {
#if HAVE_THREAD_PF

        const int num_threads = s_num_threads >= 0 ? s_num_threads : my::getDefaultNumThreads();
        //tv::Context::instance().getPTM()->run(num_threads, first, last, func, param);

#endif // HAVE_{}

        MY_XADD(&s_in_parallel, -1);
    }
    else
    {
        MY_XADD(&s_in_parallel, -1);

        for (int i = first; i < last; ++i)
            func(i, param);
    }
}


int my::getAvailableNumThreads()
{
    return s_in_parallel == 0 ? getNumThreads() : 1;   // old, cause data race
    //return MY_XADD(&s_in_parallel, 0) == 0 ? getNumThreads() : 1; // new. No data race.
}


int my::getNumThreads(void)
{
    if (s_num_threads > 0)
        return s_num_threads;

#if HAVE_THREAD_PF

    return getDefaultNumThreads();

#else // HAVE_{}

    return 1;

#endif // HAVE_{}
}


int my::getDefaultNumThreads()
{
    static const int default_num_threads = getDefaultNumThreadsImpl();
    return default_num_threads;
}

int my::getNumberOfCPUs()
{
    static int ncpus = getNumberOfCPUsImpl();
    return ncpus;
}


int my::getMaxNumThreads()
{
#if HAVE_PARALLEL_FRAMEWORK
    return getNumberOfCPUs();
#endif
    return -1;
}