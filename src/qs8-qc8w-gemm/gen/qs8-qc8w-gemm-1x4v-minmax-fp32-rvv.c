// clang-format off
// Auto-generated file. Do not edit!
//   Template: src/qs8-gemm/rvv.c.in
//   Generator: tools/xngen
//
// Copyright 2024 SiFive, Inc.
// Copyright 2024 Microchip
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <riscv_vector.h>

#include "src/xnnpack/gemm.h"
#include "src/xnnpack/math.h"

void xnn_qs8_qc8w_gemm_minmax_fp32_ukernel_1x4v__rvv(
    size_t mr,
    size_t nc,
    size_t kc,
    const int8_t* restrict a,
    size_t a_stride,
    const void* restrict w,
    int8_t* restrict c,
    size_t cm_stride,
    size_t cn_stride,
    const union xnn_qs8_qc8w_conv_minmax_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(mr != 0);
  assert(mr <= 1);
  assert(nc != 0);
  assert(kc != 0);

  const int8_t* a0 = a;
  int8_t* c0 = c;

  const size_t nr = __riscv_vsetvlmax_e32m4();
  size_t vl = nr;

  const int32_t output_min_less_zero_point = (int32_t) params->fp32_scalar.output_min - (int32_t) params->fp32_scalar.output_zero_point;
  const int32_t output_max_less_zero_point = (int32_t) params->fp32_scalar.output_max - (int32_t) params->fp32_scalar.output_zero_point;
  const int32_t output_zero_point = params->fp32_scalar.output_zero_point;
  do {
    if XNN_UNLIKELY(nc < nr) {
      vl = __riscv_vsetvl_e32m4(nc);
    }
    nc = nc - vl;

    vint32m4_t vacc0 = __riscv_vle32_v_i32m4((const int32_t*)w, vl);

    w = (const int32_t*) w + nr;

    size_t k = kc;
    do {
      const int32_t va0 = (int32_t) *a0++;

      const vint8m1_t vb = __riscv_vle8_v_i8m1((const int8_t*) w, vl);
      const vint32m4_t vb0 = __riscv_vsext_vf4(vb, vl);

      w = (const int8_t*) w + nr;

      vacc0 = __riscv_vmacc_vx_i32m4(vacc0, va0, vb0, vl);

      k -= sizeof(int8_t);
    } while (k != 0);

    vfloat32m4_t vfacc0 = __riscv_vfcvt_f_x_v_f32m4(vacc0, vl);

    const vfloat32m4_t vscale = __riscv_vle32_v_f32m4((const float*) w, vl);
    vfacc0 = __riscv_vfmul_vv_f32m4(vfacc0, vscale, vl);
    w = (const float*) w + nr;

    vfacc0 = __riscv_vfmax_vf_f32m4(vfacc0, output_min_less_zero_point, vl);
    vfacc0 = __riscv_vfmin_vf_f32m4(vfacc0, output_max_less_zero_point, vl);

    vint16m2_t vout0 = __riscv_vfncvt_x(vfacc0, vl);

    vout0 = __riscv_vadd_vx_i16m2(vout0, (int16_t) output_zero_point, vl);

    vint8m1_t vout80 = __riscv_vncvt_x_x_w_i8m1(vout0, vl);

    __riscv_vse8_v_i8m1(c0, vout80, vl);
    c0 = (int8_t*) ((uintptr_t) c0 + cn_stride);

    a0 = (const int8_t*) ((uintptr_t) a0 - kc);

  } while (nc != 0);
}
