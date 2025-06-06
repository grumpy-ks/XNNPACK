// Copyright 2024 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <algorithm>
#include <cmath>
#include <functional>
#include <random>
#include <vector>

#include "bench/utils.h"
#include "include/xnnpack.h"
#include "src/xnnpack/buffer.h"
#include "src/xnnpack/common.h"
#include "src/xnnpack/math.h"
#include "src/xnnpack/microfnptr.h"
#include "src/xnnpack/microparams-init.h"
#include "src/xnnpack/reduce.h"
#include <benchmark/benchmark.h>

static void f16_rminmax(
    benchmark::State& state, xnn_f16_reduce_ukernel_fn rminmax,
    xnn_init_f16_default_params_fn init_params = nullptr,
    benchmark::utils::IsaCheckFunction isa_check = nullptr) {
  if (isa_check != nullptr && !isa_check(state)) {
    return;
  }

  const size_t elements = state.range(0);

  std::random_device random_device;
  auto rng = std::mt19937(random_device());
  auto f32rng = std::bind(std::uniform_real_distribution<float>(-1.0f, 1.0f),
                          std::ref(rng));

  xnnpack::Buffer<xnn_float16, XNN_ALLOCATION_ALIGNMENT> input(elements);
  std::generate(input.begin(), input.end(), f32rng);

  xnn_f16_default_params params;
  if (init_params != nullptr) {
    init_params(&params);
  }

  xnn_float16 output[2];
  for (auto _ : state) {
    rminmax(elements * sizeof(xnn_float16), input.data(), output, &params);
  }

  const uint64_t cpu_frequency = benchmark::utils::GetCurrentCpuFrequency();
  if (cpu_frequency != 0) {
    state.counters["cpufreq"] = cpu_frequency;
  }

  const size_t elements_per_iteration = elements;
  state.counters["elements"] =
      benchmark::Counter(uint64_t(state.iterations()) * elements_per_iteration,
                         benchmark::Counter::kIsRate);

  const size_t bytes_per_iteration = elements * sizeof(xnn_float16);
  state.counters["bytes"] =
      benchmark::Counter(uint64_t(state.iterations()) * bytes_per_iteration,
                         benchmark::Counter::kIsRate);
}

#if XNN_ARCH_ARM || XNN_ARCH_ARM64
BENCHMARK_CAPTURE(f16_rminmax, neonfp16arith_u8,
                  xnn_f16_rminmax_ukernel__neonfp16arith_u8,
                  /*init_params=*/nullptr, benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, neonfp16arith_u16_acc2,
                  xnn_f16_rminmax_ukernel__neonfp16arith_u16_acc2,
                  /*init_params=*/nullptr, benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, neonfp16arith_u24_acc3,
                  xnn_f16_rminmax_ukernel__neonfp16arith_u24_acc3,
                  /*init_params=*/nullptr, benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, neonfp16arith_u32_acc2,
                  xnn_f16_rminmax_ukernel__neonfp16arith_u32_acc2,
                  /*init_params=*/nullptr, benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, neonfp16arith_u32_acc4,
                  xnn_f16_rminmax_ukernel__neonfp16arith_u32_acc4,
                  /*init_params=*/nullptr, benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
#endif  // XNN_ARCH_ARM || XNN_ARCH_ARM64

#if XNN_ENABLE_AVX512FP16 && (XNN_ARCH_X86 || XNN_ARCH_X86_64)
BENCHMARK_CAPTURE(f16_rminmax, avx512fp16_u32,
                  xnn_f16_rminmax_ukernel__avx512fp16_u32,
                  /*init_params=*/nullptr, benchmark::utils::CheckAVX512FP16)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, avx512fp16_u64_acc2,
                  xnn_f16_rminmax_ukernel__avx512fp16_u64_acc2,
                  /*init_params=*/nullptr, benchmark::utils::CheckAVX512FP16)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, avx512fp16_u96_acc3,
                  xnn_f16_rminmax_ukernel__avx512fp16_u96_acc3,
                  /*init_params=*/nullptr, benchmark::utils::CheckAVX512FP16)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, avx512fp16_u128_acc2,
                  xnn_f16_rminmax_ukernel__avx512fp16_u128_acc2,
                  /*init_params=*/nullptr, benchmark::utils::CheckAVX512FP16)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, avx512fp16_u128_acc4,
                  xnn_f16_rminmax_ukernel__avx512fp16_u128_acc4,
                  /*init_params=*/nullptr, benchmark::utils::CheckAVX512FP16)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
#endif  // XNN_ENABLE_AVX512FP16 && (XNN_ARCH_X86 || XNN_ARCH_X86_64)

#if XNN_ENABLE_AVX512SKX && (XNN_ARCH_X86 || XNN_ARCH_X86_64)
BENCHMARK_CAPTURE(f16_rminmax, avx512skx_u16,
                  xnn_f16_rminmax_ukernel__avx512skx_u16,
                  /*init_params=*/nullptr, benchmark::utils::CheckAVX512SKX)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, avx512skx_u32_acc2,
                  xnn_f16_rminmax_ukernel__avx512skx_u32_acc2,
                  /*init_params=*/nullptr, benchmark::utils::CheckAVX512SKX)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, avx512skx_u48_acc3,
                  xnn_f16_rminmax_ukernel__avx512skx_u48_acc3,
                  /*init_params=*/nullptr, benchmark::utils::CheckAVX512SKX)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, avx512skx_u64_acc2,
                  xnn_f16_rminmax_ukernel__avx512skx_u64_acc2,
                  /*init_params=*/nullptr, benchmark::utils::CheckAVX512SKX)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, avx512skx_u64_acc4,
                  xnn_f16_rminmax_ukernel__avx512skx_u64_acc4,
                  /*init_params=*/nullptr, benchmark::utils::CheckAVX512SKX)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
#endif  // XNN_ENABLE_AVX512SKX && (XNN_ARCH_X86 || XNN_ARCH_X86_64)

BENCHMARK_CAPTURE(f16_rminmax, scalar_u1, xnn_f16_rminmax_ukernel__scalar_u1)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, scalar_u2_acc2,
                  xnn_f16_rminmax_ukernel__scalar_u2_acc2)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, scalar_u3_acc3,
                  xnn_f16_rminmax_ukernel__scalar_u3_acc3)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, scalar_u4_acc2,
                  xnn_f16_rminmax_ukernel__scalar_u4_acc2)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();
BENCHMARK_CAPTURE(f16_rminmax, scalar_u4_acc4,
                  xnn_f16_rminmax_ukernel__scalar_u4_acc4)
    ->Apply(benchmark::utils::ReductionParameters<xnn_float16>)
    ->UseRealTime();

#ifndef XNNPACK_BENCHMARK_NO_MAIN
XNN_BENCHMARK_MAIN();
#endif
