#ifndef _TENSOR_TBLIS_GEMM_HPP_
#define _TENSOR_TBLIS_GEMM_HPP_

#include <type_traits>

#include "tblis.hpp"

#include "util/util.hpp"

namespace tblis
{
namespace blis_like
{

template <typename Child, typename... Children>
struct GEMM
{
    template <typename T> using run =
        typename Child::template run<T, Children...>;
};

template <template <typename> class MT, template <typename> class NT>
using MacroKernel = GEMM<PartitionN<NT>,
                         PartitionM<MT>,
                         MicroKernel<MT,NT>>;

using DefaultGEMM = GEMM<PartitionN<NC>,
                         PartitionK<KC>,
                         PackB<NR,KR>,
                         PartitionM<MC>,
                         PackA<MR,KR>,
                         MacroKernel<MR,NR>>;

template <typename T, typename MatrixA, typename MatrixB, typename MatrixC>
void tblis_gemm(T alpha, const MatrixA& A, const MatrixB& B, T beta, MatrixC&& C)
{
    MatrixA Av;
    MatrixB Bv;
    typename std::decay<MatrixC>::type Cv;

    ViewNoTranspose(const_cast<MatrixA&>(A), Av);
    ViewNoTranspose(const_cast<MatrixB&>(B), Bv);
    ViewNoTranspose(                     C , Cv);

    ASSERT(Av.length() == Cv.length(), "m dimension does not match");
    ASSERT(Av.width() == Bv.length(), "k dimension does not match");
    ASSERT(Bv.width() == Cv.width(), "n dimension does not match");

    DefaultGEMM::run<T>()(alpha, Av, Bv, beta, Cv);
}

}
}

#endif
