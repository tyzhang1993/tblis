#include "tblis.hpp"

namespace tblis
{

template <typename T>
static void tblis_setv_ref(idx_type n, T alpha, T* restrict A, stride_type inc_A)
{
    if (n == 0) return;

    if (inc_A == 1)
    {
        for (idx_type i = 0;i < n;i++)
        {
            A[i] = alpha;
        }
    }
    else
    {
        for (idx_type i = 0;i < n;i++)
        {
            (*A) = alpha;
            A += inc_A;
        }
    }
}

template <typename T>
void tblis_setv(T alpha, row_view<T> A)
{
    if (alpha == T(0))
    {
        tblis_zerov(A);
    }
    else
    {
        tblis_setv_ref(A.length(), alpha, A.data(), A.stride());
    }
}

template <typename T>
void tblis_setv(idx_type n, T alpha, T* A, stride_type inc_A)
{
    if (alpha == T(0))
    {
        tblis_zerov(n, A, inc_A);
    }
    else
    {
        tblis_setv_ref(n, alpha, A, inc_A);
    }
}

#define INSTANTIATE_FOR_TYPE(T) \
template void tblis_setv(idx_type n, T alpha, T* A, stride_type inc_A); \
template void tblis_setv(T alpha, row_view<T> A);
#include "tblis_instantiate_for_types.hpp"

}