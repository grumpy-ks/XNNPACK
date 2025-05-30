// clang-format off
// Auto-generated file. Do not edit!
//   Template: src/f32-vrsqrt/avx512f-rsqrt.c.in
//   Generator: tools/xngen
//
// Copyright 2024 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <immintrin.h>

#include "src/xnnpack/common.h"
#include "src/xnnpack/vunary.h"

// In the following, we do a single Newton-Raphson step on the equation
// $x^{-2} - a$, which expands to:
//
//  $$x_{k+1} = 0.5 * x_k * (3.0 - a * x_k^2)$$
//
// So we do the following steps:
//
//  1. t0 = x_k
//  2. t1 = t0 * t0       (x_k^2)
//  3. t3 = a * t1 - 3.0  (a * x_k^2 - 3.0)
//  4. t4 = 0.5 * t0      (-0.5 * x_k)
//  5. y  = t3 * t4       ((-0.5 * x_k) * (a * x_k^2 - 3.0))
//
// Where $x_k$ is the original 14-bit approximation and `y` contains the final
// 23-bit approximation $x_{k+1}$.


void xnn_f32_vrsqrt_ukernel__avx512f_rsqrt_u64(
    size_t batch,
    const float* input,
    float* output,
    const struct xnn_f32_default_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(batch != 0);
  assert(batch % sizeof(float) == 0);
  assert(input != NULL);
  assert(output != NULL);

  // Constants for the Newton-Raphson iteration.
  const __m512 vthree = _mm512_set1_ps(3.0f);
  const __m512 vneg_half = _mm512_set1_ps(-0.5f);

  for (; batch >= 64 * sizeof(float); batch -= 64 * sizeof(float)) {
    const __m512 vx0 = _mm512_loadu_ps(input);
    const __m512 vx1 = _mm512_loadu_ps(input + 16);
    const __m512 vx2 = _mm512_loadu_ps(input + 32);
    const __m512 vx3 = _mm512_loadu_ps(input + 48);
    input += 64;

    // Generate the initial 14-bit approximation.
    const __m512 vt0_0 = _mm512_rsqrt14_ps(vx0);
    const __m512 vt0_1 = _mm512_rsqrt14_ps(vx1);
    const __m512 vt0_2 = _mm512_rsqrt14_ps(vx2);
    const __m512 vt0_3 = _mm512_rsqrt14_ps(vx3);

    // Do a single Newton-Raphson step as described above.
    const __m512 vt1_0 = _mm512_mul_ps(vt0_0, vt0_0);
    const __m512 vt1_1 = _mm512_mul_ps(vt0_1, vt0_1);
    const __m512 vt1_2 = _mm512_mul_ps(vt0_2, vt0_2);
    const __m512 vt1_3 = _mm512_mul_ps(vt0_3, vt0_3);
    const __m512 vt3_0 = _mm512_fmsub_ps(vx0, vt1_0, vthree);
    const __m512 vt3_1 = _mm512_fmsub_ps(vx1, vt1_1, vthree);
    const __m512 vt3_2 = _mm512_fmsub_ps(vx2, vt1_2, vthree);
    const __m512 vt3_3 = _mm512_fmsub_ps(vx3, vt1_3, vthree);
    const __m512 vt4_0 = _mm512_mul_ps(vneg_half, vt0_0);
    const __m512 vt4_1 = _mm512_mul_ps(vneg_half, vt0_1);
    const __m512 vt4_2 = _mm512_mul_ps(vneg_half, vt0_2);
    const __m512 vt4_3 = _mm512_mul_ps(vneg_half, vt0_3);
    const __m512 vy0 = _mm512_mul_ps(vt3_0, vt4_0);
    const __m512 vy1 = _mm512_mul_ps(vt3_1, vt4_1);
    const __m512 vy2 = _mm512_mul_ps(vt3_2, vt4_2);
    const __m512 vy3 = _mm512_mul_ps(vt3_3, vt4_3);

    // Store the results.
    _mm512_storeu_ps(output, vy0);
    _mm512_storeu_ps(output + 16, vy1);
    _mm512_storeu_ps(output + 32, vy2);
    _mm512_storeu_ps(output + 48, vy3);
    output += 64;
  }
  for (; batch >= 16 * sizeof(float); batch -= 16 * sizeof(float)) {
    const __m512 vx = _mm512_loadu_ps(input);
    input += 16;

    // Generate the initial 14-bit approximation.
    const __m512 vt0 = _mm512_rsqrt14_ps(vx);

    // Do a single Newton-Raphson step as described above.
    const __m512 vt1 = _mm512_mul_ps(vt0, vt0);
    const __m512 vt3 = _mm512_fmsub_ps(vx, vt1, vthree);
    const __m512 vt4 = _mm512_mul_ps(vneg_half, vt0);
    const __m512 vy = _mm512_mul_ps(vt3, vt4);

    _mm512_storeu_ps(output, vy);
    output += 16;
  }
  if XNN_UNLIKELY(batch != 0) {
    assert(batch >= 1 * sizeof(float));
    assert(batch <= 15 * sizeof(float));
    // Prepare mask for valid 32-bit elements (depends on batch).
    batch >>= XNN_LOG2_SIZEOF_FLOAT;
    const __mmask16 vmask = _cvtu32_mask16((uint32_t) ((UINT32_C(1) << batch) - UINT32_C(1)));
    const __m512 vx = _mm512_maskz_loadu_ps(vmask, input);

    // Generate the initial 14-bit approximation.
    const __m512 vt0 = _mm512_rsqrt14_ps(vx);

    // Do a single Newton-Raphson step as described above.
    const __m512 vt1 = _mm512_mul_ps(vt0, vt0);
    const __m512 vt3 = _mm512_fmsub_ps(vx, vt1, vthree);
    const __m512 vt4 = _mm512_mul_ps(vneg_half, vt0);
    __m512 vy = _mm512_mul_ps(vt3, vt4);

    _mm512_mask_storeu_ps(output, vmask, vy);
  }
}
