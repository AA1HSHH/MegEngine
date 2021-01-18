/**
 * \file dnn/src/x86/elemwise_helper/kimpl/fuse_mul_add3.h
 * MegEngine is Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Copyright (c) 2014-2021 Megvii Inc. All rights reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT ARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */
#pragma once

#include "src/x86/elemwise_helper/kimpl/op_ternary_base.h"

namespace megdnn {
namespace x86 {

template <SIMDType simd_type, typename src_ctype,
          typename dst_ctype = src_ctype>
struct FuseMulAdd3OpBase : TernaryOpBase<simd_type, src_ctype, dst_ctype> {
    using TernaryOpBase<simd_type, src_ctype, dst_ctype>::TernaryOpBase;
    void operator()(const src_ctype& src0, const src_ctype& src1,
                    const src_ctype src2, dst_ctype* dst) const {
        *dst = operator()(src0, src1, src2);
    }
    dst_ctype operator()(const src_ctype& src0, const src_ctype& src1,
                         const src_ctype& src2) const {
        return (src0 * src1) + src2;
    }
};

template <SIMDType simd_type, typename src_ctype,
          typename dst_ctype = src_ctype>
struct FuseMulAdd3Op;

#define OP(_ctype, _simd_type, _simd_target, _simd_data_type,                 \
           _simd_data_type2, _ptr_type, _func_prefix, _func_suffix1,          \
           _func_suffix2, _simd_width)                                        \
    template <>                                                               \
    struct FuseMulAdd3Op<_simd_type, _ctype>                                  \
            : FuseMulAdd3OpBase<_simd_type, _ctype> {                         \
        using FuseMulAdd3OpBase::FuseMulAdd3OpBase;                           \
        using FuseMulAdd3OpBase::operator();                                  \
        constexpr static size_t SIMD_WIDTH = _simd_width;                     \
        MEGDNN_ATTRIBUTE_TARGET(_simd_target)                                 \
        void operator()(const _simd_data_type2& src0,                         \
                        const _simd_data_type2& src1,                         \
                        const _simd_data_type2& src2, _ctype* dst) const {    \
            auto vitem = operator()(src0, src1, src2);                        \
            _##_func_prefix##_storeu_##_func_suffix2(                         \
                    reinterpret_cast<_ptr_type*>(dst), vitem.val[0]);         \
            _##_func_prefix##_storeu_##_func_suffix2(                         \
                    reinterpret_cast<_ptr_type*>(dst + SIMD_WIDTH),           \
                    vitem.val[1]);                                            \
        }                                                                     \
        MEGDNN_ATTRIBUTE_TARGET(_simd_target)                                 \
        _simd_data_type2 operator()(const _simd_data_type2& src0,             \
                                    const _simd_data_type2& src1,             \
                                    const _simd_data_type2& src2) const {     \
            auto vitem0 = _##_func_prefix##_mul_##_func_suffix1(src0.val[0],  \
                                                                src1.val[0]); \
            vitem0 = _##_func_prefix##_add_##_func_suffix1(src2.val[0],       \
                                                           vitem0);           \
            auto vitem1 = _##_func_prefix##_mul_##_func_suffix1(src0.val[1],  \
                                                                src1.val[1]); \
            vitem1 = _##_func_prefix##_add_##_func_suffix1(src2.val[1],       \
                                                           vitem1);           \
            return {{vitem0, vitem1}};                                        \
        }                                                                     \
        MEGDNN_ATTRIBUTE_TARGET(_simd_target)                                 \
        _simd_data_type operator()(const _simd_data_type& src0,               \
                                   const _simd_data_type& src1,               \
                                   const _simd_data_type& src2) const {       \
            auto vitem = _##_func_prefix##_mul_##_func_suffix1(src0, src1);   \
            return _##_func_prefix##_add_##_func_suffix1(src2, vitem);        \
        }                                                                     \
    };

OP(dt_float32, SIMDType::SSE4_2, "sse4.2", __m128, __m128x2, float, mm, ps, ps,
   4)
OP(dt_int32, SIMDType::SSE4_2, "sse4.2", __m128i, __m128ix2, __m128i, mm, epi32,
   si128, 4)

OP(dt_float32, SIMDType::AVX2, "avx2", __m256, __m256x2, float, mm256, ps, ps,
   8)
OP(dt_int32, SIMDType::AVX2, "avx2", __m256i, __m256ix2, __m256i, mm256, epi32,
   si256, 8)
#undef OP

}  // namespace x86
}  // namespace megdnn

// vim: syntax=cpp.doxygen
