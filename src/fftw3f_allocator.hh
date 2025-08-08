
#pragma once

#include <cstdlib>
#include <limits>
#include <type_traits>
#include <stdexcept>
#include <fftw3.h>


// Custom allocator for FFTW3 data types
// Allows FFTW3 to control memory alignment for improved internal vectorization 
template <typename T>
class fftwf_allocator {
public:
    using value_type = T;
    using size_type = std::size_t;
    fftwf_allocator() noexcept = default;
    template <typename U> fftwf_allocator(const fftwf_allocator<U>&) noexcept {}
    void deallocate(T* p, size_type n) noexcept { fftwf_free(p); }
    T* allocate(size_type N) {
        void* p = fftwf_malloc(N * sizeof(T));
        if ( (!p) || (N * sizeof(T) > std::numeric_limits<size_type>::max()) ) { throw std::bad_alloc(); }
        return reinterpret_cast<T*>(p);
    }
};

template <typename T1, typename T2>
bool operator==(const fftwf_allocator<T1>&, const fftwf_allocator<T2>&) noexcept {
    return true; 
}

template <typename T1, typename T2>
bool operator!=(const fftwf_allocator<T1>&, const fftwf_allocator<T2>&) noexcept {
    return false;
}