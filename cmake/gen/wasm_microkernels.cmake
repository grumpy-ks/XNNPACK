# Copyright 2022 Google LLC
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.
#
# Description: microkernel filename lists for wasm
#
# Auto-generated file. Do not edit!
#   Generator: tools/update-microkernels.py


SET(PROD_WASM_MICROKERNEL_SRCS
  src/f32-dwconv/gen/f32-dwconv-3p1c-minmax-wasm-acc2.c
  src/f32-dwconv/gen/f32-dwconv-4p1c-minmax-wasm-acc2.c
  src/f32-dwconv/gen/f32-dwconv-9p1c-minmax-wasm-acc2.c
  src/f32-dwconv/gen/f32-dwconv-25p1c-minmax-wasm-acc2.c
  src/f32-gemm/gen/f32-gemm-1x4-minmax-wasm.c
  src/f32-gemm/gen/f32-gemm-1x4-relu-wasm.c
  src/f32-gemm/gen/f32-gemm-4x2-minmax-wasm.c
  src/f32-gemm/gen/f32-gemm-4x4-minmax-wasm.c
  src/f32-gemm/gen/f32-gemm-4x4-relu-wasm.c
  src/f32-igemm/gen/f32-igemm-1x4-minmax-wasm.c
  src/f32-igemm/gen/f32-igemm-1x4-relu-wasm.c
  src/f32-igemm/gen/f32-igemm-4x2-minmax-wasm.c
  src/f32-igemm/gen/f32-igemm-4x4-minmax-wasm.c
  src/f32-igemm/gen/f32-igemm-4x4-relu-wasm.c
  src/f32-qc4w-gemm/gen/f32-qc4w-gemm-1x4-minmax-wasm.c
  src/f32-qc4w-gemm/gen/f32-qc4w-gemm-4x4-minmax-wasm.c
  src/f32-qc8w-gemm/gen/f32-qc8w-gemm-1x4-minmax-wasm.c
  src/f32-qc8w-gemm/gen/f32-qc8w-gemm-4x4-minmax-wasm.c
  src/f32-qs8-vcvt/gen/f32-qs8-vcvt-wasm-fmagic-u4.c
  src/f32-qu8-vcvt/gen/f32-qu8-vcvt-wasm-fmagic-u4.c
  src/f32-rminmax/gen/f32-rmax-wasm-u4-acc4.c
  src/f32-rminmax/gen/f32-rmin-wasm-u4-acc4.c
  src/f32-rminmax/gen/f32-rminmax-wasm-u4-acc4.c
  src/f32-vbinary/gen/f32-vadd-wasm-u8.c
  src/f32-vbinary/gen/f32-vaddc-wasm-u8.c
  src/f32-vbinary/gen/f32-vdiv-wasm-u8.c
  src/f32-vbinary/gen/f32-vdivc-wasm-u8.c
  src/f32-vbinary/gen/f32-vmax-wasm-u8.c
  src/f32-vbinary/gen/f32-vmaxc-wasm-u8.c
  src/f32-vbinary/gen/f32-vmin-wasm-u8.c
  src/f32-vbinary/gen/f32-vminc-wasm-u8.c
  src/f32-vbinary/gen/f32-vmul-wasm-u8.c
  src/f32-vbinary/gen/f32-vmulc-wasm-u8.c
  src/f32-vbinary/gen/f32-vprelu-wasm-u8.c
  src/f32-vbinary/gen/f32-vpreluc-wasm-u8.c
  src/f32-vbinary/gen/f32-vrdivc-wasm-u8.c
  src/f32-vbinary/gen/f32-vrpreluc-wasm-u8.c
  src/f32-vbinary/gen/f32-vrsubc-wasm-u8.c
  src/f32-vbinary/gen/f32-vsub-wasm-u8.c
  src/f32-vbinary/gen/f32-vsubc-wasm-u8.c
  src/f32-velu/gen/f32-velu-wasm-rr2-p6-u6.c
  src/f32-vmulcaddc/gen/f32-vmulcaddc-c1-minmax-wasm-2x.c
  src/qd8-f32-qc4w-gemm/gen/qd8-f32-qc4w-gemm-1x4-minmax-wasm.c
  src/qd8-f32-qc4w-gemm/gen/qd8-f32-qc4w-gemm-4x4-minmax-wasm.c
  src/qd8-f32-qc8w-gemm/gen/qd8-f32-qc8w-gemm-1x4-minmax-wasm.c
  src/qd8-f32-qc8w-gemm/gen/qd8-f32-qc8w-gemm-4x4-minmax-wasm.c
  src/qd8-f32-qc8w-igemm/gen/qd8-f32-qc8w-igemm-1x4-minmax-wasm.c
  src/qd8-f32-qc8w-igemm/gen/qd8-f32-qc8w-igemm-4x4-minmax-wasm.c
  src/qs8-dwconv/gen/qs8-dwconv-9p2c-minmax-fp32-wasm-fmagic.c
  src/qs8-dwconv/gen/qs8-dwconv-25p2c-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-dwconv/gen/qs8-qc8w-dwconv-3p2c-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-dwconv/gen/qs8-qc8w-dwconv-9p2c-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-dwconv/gen/qs8-qc8w-dwconv-25p2c-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-gemm/gen/qs8-qc8w-gemm-1x4-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-gemm/gen/qs8-qc8w-gemm-4x4-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-igemm/gen/qs8-qc8w-igemm-1x4-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-igemm/gen/qs8-qc8w-igemm-4x4-minmax-fp32-wasm-fmagic.c
  src/qu8-dwconv/gen/qu8-dwconv-9p2c-minmax-fp32-wasm-fmagic.c
  src/qu8-dwconv/gen/qu8-dwconv-25p2c-minmax-fp32-wasm-fmagic.c
  src/qu8-gemm/gen/qu8-gemm-1x4-minmax-fp32-wasm-fmagic.c
  src/qu8-gemm/gen/qu8-gemm-4x4-minmax-fp32-wasm-fmagic.c
  src/qu8-igemm/gen/qu8-igemm-1x4-minmax-fp32-wasm-fmagic.c
  src/qu8-igemm/gen/qu8-igemm-4x4-minmax-fp32-wasm-fmagic.c)

SET(NON_PROD_WASM_MICROKERNEL_SRCS
  src/f32-dwconv/gen/f32-dwconv-3p1c-minmax-wasm.c
  src/f32-dwconv/gen/f32-dwconv-3p2c-minmax-wasm-acc2.c
  src/f32-dwconv/gen/f32-dwconv-3p2c-minmax-wasm.c
  src/f32-dwconv/gen/f32-dwconv-4p1c-minmax-wasm.c
  src/f32-dwconv/gen/f32-dwconv-4p2c-minmax-wasm-acc2.c
  src/f32-dwconv/gen/f32-dwconv-4p2c-minmax-wasm.c
  src/f32-dwconv/gen/f32-dwconv-9p1c-minmax-wasm.c
  src/f32-dwconv/gen/f32-dwconv-9p2c-minmax-wasm-acc2.c
  src/f32-dwconv/gen/f32-dwconv-9p2c-minmax-wasm.c
  src/f32-dwconv/gen/f32-dwconv-25p1c-minmax-wasm.c
  src/f32-dwconv/gen/f32-dwconv-25p2c-minmax-wasm-acc2.c
  src/f32-dwconv/gen/f32-dwconv-25p2c-minmax-wasm.c
  src/f32-gemm/gen/f32-gemm-2x4-minmax-wasm.c
  src/f32-gemm/gen/f32-gemm-2x4-relu-wasm.c
  src/f32-gemm/gen/f32-gemm-4x2-relu-wasm.c
  src/f32-gemminc/gen/f32-gemminc-1x4-minmax-wasm.c
  src/f32-gemminc/gen/f32-gemminc-2x4-minmax-wasm.c
  src/f32-gemminc/gen/f32-gemminc-4x4-minmax-wasm.c
  src/f32-igemm/gen/f32-igemm-2x4-minmax-wasm.c
  src/f32-igemm/gen/f32-igemm-2x4-relu-wasm.c
  src/f32-igemm/gen/f32-igemm-4x2-relu-wasm.c
  src/f32-qc4w-gemm/gen/f32-qc4w-gemm-2x4-minmax-wasm.c
  src/f32-qc4w-gemm/gen/f32-qc4w-gemm-4x2-minmax-wasm.c
  src/f32-qc8w-gemm/gen/f32-qc8w-gemm-1x4-relu-wasm.c
  src/f32-qc8w-gemm/gen/f32-qc8w-gemm-2x4-minmax-wasm.c
  src/f32-qc8w-gemm/gen/f32-qc8w-gemm-2x4-relu-wasm.c
  src/f32-qc8w-gemm/gen/f32-qc8w-gemm-4x2-minmax-wasm.c
  src/f32-qc8w-gemm/gen/f32-qc8w-gemm-4x2-relu-wasm.c
  src/f32-qc8w-gemm/gen/f32-qc8w-gemm-4x4-relu-wasm.c
  src/f32-qs8-vcvt/gen/f32-qs8-vcvt-wasm-fmagic-u1.c
  src/f32-qs8-vcvt/gen/f32-qs8-vcvt-wasm-fmagic-u2.c
  src/f32-qs8-vcvt/gen/f32-qs8-vcvt-wasm-fmagic-u3.c
  src/f32-qu8-vcvt/gen/f32-qu8-vcvt-wasm-fmagic-u1.c
  src/f32-qu8-vcvt/gen/f32-qu8-vcvt-wasm-fmagic-u2.c
  src/f32-qu8-vcvt/gen/f32-qu8-vcvt-wasm-fmagic-u3.c
  src/f32-rminmax/gen/f32-rmax-wasm-u1.c
  src/f32-rminmax/gen/f32-rmax-wasm-u2-acc2.c
  src/f32-rminmax/gen/f32-rmax-wasm-u3-acc3.c
  src/f32-rminmax/gen/f32-rmax-wasm-u4-acc2.c
  src/f32-rminmax/gen/f32-rmin-wasm-u1.c
  src/f32-rminmax/gen/f32-rmin-wasm-u2-acc2.c
  src/f32-rminmax/gen/f32-rmin-wasm-u3-acc3.c
  src/f32-rminmax/gen/f32-rmin-wasm-u4-acc2.c
  src/f32-rminmax/gen/f32-rminmax-wasm-u1.c
  src/f32-rminmax/gen/f32-rminmax-wasm-u2-acc2.c
  src/f32-rminmax/gen/f32-rminmax-wasm-u3-acc3.c
  src/f32-rminmax/gen/f32-rminmax-wasm-u4-acc2.c
  src/f32-vbinary/gen/f32-vadd-wasm-u1.c
  src/f32-vbinary/gen/f32-vadd-wasm-u2.c
  src/f32-vbinary/gen/f32-vadd-wasm-u4.c
  src/f32-vbinary/gen/f32-vaddc-wasm-u1.c
  src/f32-vbinary/gen/f32-vaddc-wasm-u2.c
  src/f32-vbinary/gen/f32-vaddc-wasm-u4.c
  src/f32-vbinary/gen/f32-vdiv-wasm-u1.c
  src/f32-vbinary/gen/f32-vdiv-wasm-u2.c
  src/f32-vbinary/gen/f32-vdiv-wasm-u4.c
  src/f32-vbinary/gen/f32-vdivc-wasm-u1.c
  src/f32-vbinary/gen/f32-vdivc-wasm-u2.c
  src/f32-vbinary/gen/f32-vdivc-wasm-u4.c
  src/f32-vbinary/gen/f32-vmax-wasm-u1.c
  src/f32-vbinary/gen/f32-vmax-wasm-u2.c
  src/f32-vbinary/gen/f32-vmax-wasm-u4.c
  src/f32-vbinary/gen/f32-vmaxc-wasm-u1.c
  src/f32-vbinary/gen/f32-vmaxc-wasm-u2.c
  src/f32-vbinary/gen/f32-vmaxc-wasm-u4.c
  src/f32-vbinary/gen/f32-vmin-wasm-u1.c
  src/f32-vbinary/gen/f32-vmin-wasm-u2.c
  src/f32-vbinary/gen/f32-vmin-wasm-u4.c
  src/f32-vbinary/gen/f32-vminc-wasm-u1.c
  src/f32-vbinary/gen/f32-vminc-wasm-u2.c
  src/f32-vbinary/gen/f32-vminc-wasm-u4.c
  src/f32-vbinary/gen/f32-vmul-wasm-u1.c
  src/f32-vbinary/gen/f32-vmul-wasm-u2.c
  src/f32-vbinary/gen/f32-vmul-wasm-u4.c
  src/f32-vbinary/gen/f32-vmulc-wasm-u1.c
  src/f32-vbinary/gen/f32-vmulc-wasm-u2.c
  src/f32-vbinary/gen/f32-vmulc-wasm-u4.c
  src/f32-vbinary/gen/f32-vprelu-wasm-u1.c
  src/f32-vbinary/gen/f32-vprelu-wasm-u2.c
  src/f32-vbinary/gen/f32-vprelu-wasm-u4.c
  src/f32-vbinary/gen/f32-vpreluc-wasm-u1.c
  src/f32-vbinary/gen/f32-vpreluc-wasm-u2.c
  src/f32-vbinary/gen/f32-vpreluc-wasm-u4.c
  src/f32-vbinary/gen/f32-vrdivc-wasm-u1.c
  src/f32-vbinary/gen/f32-vrdivc-wasm-u2.c
  src/f32-vbinary/gen/f32-vrdivc-wasm-u4.c
  src/f32-vbinary/gen/f32-vrpreluc-wasm-u1.c
  src/f32-vbinary/gen/f32-vrpreluc-wasm-u2.c
  src/f32-vbinary/gen/f32-vrpreluc-wasm-u4.c
  src/f32-vbinary/gen/f32-vrsubc-wasm-u1.c
  src/f32-vbinary/gen/f32-vrsubc-wasm-u2.c
  src/f32-vbinary/gen/f32-vrsubc-wasm-u4.c
  src/f32-vbinary/gen/f32-vsub-wasm-u1.c
  src/f32-vbinary/gen/f32-vsub-wasm-u2.c
  src/f32-vbinary/gen/f32-vsub-wasm-u4.c
  src/f32-vbinary/gen/f32-vsubc-wasm-u1.c
  src/f32-vbinary/gen/f32-vsubc-wasm-u2.c
  src/f32-vbinary/gen/f32-vsubc-wasm-u4.c
  src/f32-velu/gen/f32-velu-wasm-rr2-lut16-p3-u1.c
  src/f32-velu/gen/f32-velu-wasm-rr2-lut16-p3-u2.c
  src/f32-velu/gen/f32-velu-wasm-rr2-lut16-p3-u3.c
  src/f32-velu/gen/f32-velu-wasm-rr2-lut16-p3-u4.c
  src/f32-velu/gen/f32-velu-wasm-rr2-lut16-p3-u5.c
  src/f32-velu/gen/f32-velu-wasm-rr2-lut16-p3-u6.c
  src/f32-velu/gen/f32-velu-wasm-rr2-p6-u1.c
  src/f32-velu/gen/f32-velu-wasm-rr2-p6-u2.c
  src/f32-velu/gen/f32-velu-wasm-rr2-p6-u3.c
  src/f32-velu/gen/f32-velu-wasm-rr2-p6-u4.c
  src/f32-velu/gen/f32-velu-wasm-rr2-p6-u5.c
  src/f32-vlrelu/gen/f32-vlrelu-wasm-u1.c
  src/f32-vlrelu/gen/f32-vlrelu-wasm-u2.c
  src/f32-vlrelu/gen/f32-vlrelu-wasm-u4.c
  src/f32-vmulcaddc/gen/f32-vmulcaddc-c2-minmax-wasm-2x.c
  src/f32-vmulcaddc/gen/f32-vmulcaddc-c4-minmax-wasm-2x.c
  src/qd8-f32-qc4w-gemm/gen/qd8-f32-qc4w-gemm-1x2-minmax-wasm.c
  src/qd8-f32-qc4w-gemm/gen/qd8-f32-qc4w-gemm-1x8-minmax-wasm.c
  src/qd8-f32-qc4w-gemm/gen/qd8-f32-qc4w-gemm-2x2-minmax-wasm.c
  src/qd8-f32-qc4w-gemm/gen/qd8-f32-qc4w-gemm-2x4-minmax-wasm.c
  src/qd8-f32-qc4w-gemm/gen/qd8-f32-qc4w-gemm-2x8-minmax-wasm.c
  src/qd8-f32-qc8w-gemm/gen/qd8-f32-qc8w-gemm-1x2-minmax-wasm.c
  src/qd8-f32-qc8w-gemm/gen/qd8-f32-qc8w-gemm-1x8-minmax-wasm.c
  src/qd8-f32-qc8w-gemm/gen/qd8-f32-qc8w-gemm-2x2-minmax-wasm.c
  src/qd8-f32-qc8w-gemm/gen/qd8-f32-qc8w-gemm-2x4-minmax-wasm.c
  src/qd8-f32-qc8w-gemm/gen/qd8-f32-qc8w-gemm-2x8-minmax-wasm.c
  src/qd8-f32-qc8w-igemm/gen/qd8-f32-qc8w-igemm-1x2-minmax-wasm.c
  src/qd8-f32-qc8w-igemm/gen/qd8-f32-qc8w-igemm-1x8-minmax-wasm.c
  src/qd8-f32-qc8w-igemm/gen/qd8-f32-qc8w-igemm-2x2-minmax-wasm.c
  src/qd8-f32-qc8w-igemm/gen/qd8-f32-qc8w-igemm-2x4-minmax-wasm.c
  src/qd8-f32-qc8w-igemm/gen/qd8-f32-qc8w-igemm-2x8-minmax-wasm.c
  src/qs8-dwconv/gen/qs8-dwconv-9p1c-minmax-fp32-wasm-fmagic.c
  src/qs8-dwconv/gen/qs8-dwconv-9p4c-minmax-fp32-wasm-fmagic.c
  src/qs8-dwconv/gen/qs8-dwconv-25p1c-minmax-fp32-wasm-fmagic.c
  src/qs8-dwconv/gen/qs8-dwconv-25p4c-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-dwconv/gen/qs8-qc8w-dwconv-9p1c-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-dwconv/gen/qs8-qc8w-dwconv-9p4c-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-dwconv/gen/qs8-qc8w-dwconv-25p1c-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-dwconv/gen/qs8-qc8w-dwconv-25p4c-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-gemm/gen/qs8-qc8w-gemm-1x2-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-gemm/gen/qs8-qc8w-gemm-2x2-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-gemm/gen/qs8-qc8w-gemm-2x4-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-gemm/gen/qs8-qc8w-gemm-3x2-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-gemm/gen/qs8-qc8w-gemm-3x4-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-gemm/gen/qs8-qc8w-gemm-4x2-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-igemm/gen/qs8-qc8w-igemm-1x2-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-igemm/gen/qs8-qc8w-igemm-2x2-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-igemm/gen/qs8-qc8w-igemm-2x4-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-igemm/gen/qs8-qc8w-igemm-3x2-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-igemm/gen/qs8-qc8w-igemm-3x4-minmax-fp32-wasm-fmagic.c
  src/qs8-qc8w-igemm/gen/qs8-qc8w-igemm-4x2-minmax-fp32-wasm-fmagic.c
  src/qu8-dwconv/gen/qu8-dwconv-9p1c-minmax-fp32-wasm-fmagic.c
  src/qu8-dwconv/gen/qu8-dwconv-9p4c-minmax-fp32-wasm-fmagic.c
  src/qu8-dwconv/gen/qu8-dwconv-25p1c-minmax-fp32-wasm-fmagic.c
  src/qu8-dwconv/gen/qu8-dwconv-25p4c-minmax-fp32-wasm-fmagic.c
  src/qu8-gemm/gen/qu8-gemm-1x2-minmax-fp32-wasm-fmagic.c
  src/qu8-gemm/gen/qu8-gemm-2x2-minmax-fp32-wasm-fmagic.c
  src/qu8-gemm/gen/qu8-gemm-2x4-minmax-fp32-wasm-fmagic.c
  src/qu8-gemm/gen/qu8-gemm-3x2-minmax-fp32-wasm-fmagic.c
  src/qu8-gemm/gen/qu8-gemm-3x4-minmax-fp32-wasm-fmagic.c
  src/qu8-gemm/gen/qu8-gemm-4x2-minmax-fp32-wasm-fmagic.c
  src/qu8-igemm/gen/qu8-igemm-1x2-minmax-fp32-wasm-fmagic.c
  src/qu8-igemm/gen/qu8-igemm-2x2-minmax-fp32-wasm-fmagic.c
  src/qu8-igemm/gen/qu8-igemm-2x4-minmax-fp32-wasm-fmagic.c
  src/qu8-igemm/gen/qu8-igemm-3x2-minmax-fp32-wasm-fmagic.c
  src/qu8-igemm/gen/qu8-igemm-3x4-minmax-fp32-wasm-fmagic.c
  src/qu8-igemm/gen/qu8-igemm-4x2-minmax-fp32-wasm-fmagic.c)

SET(ALL_WASM_MICROKERNEL_SRCS ${PROD_WASM_MICROKERNEL_SRCS} + ${NON_PROD_WASM_MICROKERNEL_SRCS})
