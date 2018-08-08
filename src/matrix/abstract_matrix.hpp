#ifndef _TBLIS_ABSTRACT_MATRIX_HPP_
#define _TBLIS_ABSTRACT_MATRIX_HPP_

#include "util/basic_types.h"
#include "util/thread.h"
#include "configs/configs.hpp"

namespace tblis
{

template <typename T>
class abstract_matrix
{
    public:
        typedef T value_type;

    protected:
        T* data_ = nullptr;
        std::array<len_type,2> tot_len_ = {};
        std::array<len_type,2> cur_len_ = {};
        std::array<len_type,2> off_ = {};

    public:
        abstract_matrix() {}

        abstract_matrix(len_type m, len_type n, T* ptr)
        : data_(ptr), tot_len_{m, n}, cur_len_{m, n} {}

        T* data() const
        {
            return data_;
        }

        T* data(T* ptr)
        {
            std::swap(ptr, data_);
            return ptr;
        }

        len_type length(unsigned dim) const
        {
            TBLIS_ASSERT(dim < 2);
            return cur_len_[dim];
        }

        len_type length(unsigned dim, len_type len)
        {
            TBLIS_ASSERT(dim < 2);
            std::swap(len, cur_len_[dim]);
            TBLIS_ASSERT(cur_len_[dim] >= 0);
            TBLIS_ASSERT(off_[dim]+cur_len_[dim] <= tot_len_[dim]);
            return len;
        }

        const std::array<len_type, 2>& lengths() const
        {
            return cur_len_;
        }

        void shift(unsigned dim, len_type n)
        {
            TBLIS_ASSERT(dim < 2);
            off_[dim] += n;
            TBLIS_ASSERT(off_[dim] >= 0);
            TBLIS_ASSERT(off_[dim]+cur_len_[dim] <= tot_len_[dim]);
        }

        void transpose()
        {
            using std::swap;
            swap(cur_len_[0], cur_len_[1]);
            swap(tot_len_[0], tot_len_[1]);
            swap(off_[0], off_[1]);
        }
};

}

#endif