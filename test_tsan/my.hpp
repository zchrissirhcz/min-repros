#pragma once

// copied from OpenCV:  https://github.com/opencv/opencv/blob/4.x/modules/core/include/opencv2/core/cvdef.h

/****************************************************************************************\
*          exchange-add operation for atomic operations on reference counters            *
\****************************************************************************************/

#ifdef MY_XADD
  // allow to use user-defined macro
#elif defined __GNUC__ || defined __clang__
#  if defined __clang__ && __clang_major__ >= 3 && !defined __ANDROID__ && !defined __EMSCRIPTEN__ && !defined(__CUDACC__)  && !defined __INTEL_COMPILER
#    ifdef __ATOMIC_ACQ_REL
#      define MY_XADD(addr, delta) __c11_atomic_fetch_add((_Atomic(int)*)(addr), delta, __ATOMIC_ACQ_REL)
#    else
#      define MY_XADD(addr, delta) __atomic_fetch_add((_Atomic(int)*)(addr), delta, 4)
#    endif
#  else
#    if defined __ATOMIC_ACQ_REL && !defined __clang__
       // version for gcc >= 4.7
#      define MY_XADD(addr, delta) (int)__atomic_fetch_add((unsigned*)(addr), (unsigned)(delta), __ATOMIC_ACQ_REL)
#    else
#      define MY_XADD(addr, delta) (int)__sync_fetch_and_add((unsigned*)(addr), (unsigned)(delta))
#    endif
#  endif
#elif defined _MSC_VER && !defined RC_INVOKED
#  include <intrin.h>
#  define MY_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
#else
  #ifdef OPENCV_FORCE_UNSAFE_XADD
    CV_INLINE int MY_XADD(int* addr, int delta) { int tmp = *addr; *addr += delta; return tmp; }
  #else
    #error "OpenCV: can't define safe MY_XADD macro for current platform (unsupported). Define MY_XADD macro through custom port header (see OPENCV_INCLUDE_PORT_FILE)"
  #endif
#endif





#define USE_PARALLEL_FOR 1
#define HAVE_THREAD_PF 1
#define HAVE_PARALLEL_FRAMEWORK 1

namespace my {

int getAvailableNumThreads();

void setNumThreads(int num_threads);
int getNumThreads();
int getDefaultNumThreads();
int getNumberOfCPUs();
int getMaxNumThreads();

typedef void (*ParallelLoopFunc)(int index, void* param);
void parallel_for_(int first, int last, const ParallelLoopFunc& func, void* param=0);


template<typename IndexType, typename Function> inline
void parallel_for(IndexType first, IndexType last, const Function& func)
{
#if USE_PARALLEL_FOR
    // fast return
    if (last - first > 1 && my::getAvailableNumThreads() > 1)
    {
        my::parallel_for_((int) first, (int) last, [](int index, void* param) {
            (*static_cast<Function*>(param))((IndexType) index);
        }, (void*) &func);
    }
    else
#endif // USE_PARALLEL_FOR
    {
        for (IndexType i = first; i < last; ++i)
            func(i);
    }
}

} // namespace my