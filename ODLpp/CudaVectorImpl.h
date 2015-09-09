#pragma once

#include <stdint.h>
#include <sstream>
#include <type_traits>

#include <ODLpp/DeviceVector.h>

template <typename T, typename Enable = void>
struct CudaVectorScalar;

template <typename T>
struct CudaVectorScalar<
    T, typename std::enable_if<std::is_integral<T>::value>::type> {
    typedef typename std::make_signed<T>::type Scalar;
};

template <typename T>
struct CudaVectorScalar<
    T, typename std::enable_if<!std::is_integral<T>::value>::type> {
    typedef T Scalar;
};

template <typename T>
class CudaVectorImpl {
   public:
    typedef typename CudaVectorScalar<T>::Scalar Scalar;

    CudaVectorImpl(size_t size);
    CudaVectorImpl(size_t size, T value);
    CudaVectorImpl(DeviceVectorPtr<T> impl);

    static DeviceVectorPtr<T> fromPointer(uintptr_t ptr, size_t size,
                                          size_t stride);

    T getItem(ptrdiff_t index) const;
    void setItem(ptrdiff_t index, T value);

    // numerical methods
    void linComb(Scalar a, const CudaVectorImpl<T>& x, Scalar b,
                 const CudaVectorImpl<T>& y);
    double dist(const CudaVectorImpl<T>& other) const;
    double norm() const;
    double inner(const CudaVectorImpl<T>& v2) const;
    void multiply(const CudaVectorImpl<T>& v1, const CudaVectorImpl<T>& v2);

    // Convenience methods
    CudaVectorImpl<T> copy() const;
    bool allEqual(const CudaVectorImpl<T>& v2) const;
    void fill(T value);

    // Implicit conversion to the data container
    operator DeviceVector<T>&();
    operator const DeviceVector<T>&() const;

    // Accessors for data
    uintptr_t dataPtr() const;
    size_t stride() const;
    size_t size() const;

    // Raw copy
    void getSliceImpl(const DeviceVector<T>& v1, int start, int stop, int step,
                      T* host_target) const;
    void setSliceImpl(DeviceVector<T>& v1, int start, int stop, int step,
                      const T* host_source, int num);

    // Members
    DeviceVectorPtr<T> _impl;

    void validateIndex(ptrdiff_t index) const;
    // Copy to ostream
    void printData(std::ostream_iterator<T>& out, int numel) const;
};
