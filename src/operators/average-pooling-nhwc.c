// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// Copyright 2019 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "include/xnnpack.h"
#include "src/xnnpack/allocator.h"
#include "src/xnnpack/common.h"
#include "src/xnnpack/compute.h"
#include "src/xnnpack/config-types.h"
#include "src/xnnpack/config.h"
#include "src/xnnpack/indirection.h"
#include "src/xnnpack/log.h"
#include "src/xnnpack/math.h"
#include "src/xnnpack/microkernel-type.h"
#include "src/xnnpack/operator-type.h"
#include "src/xnnpack/operator-utils.h"
#include "src/xnnpack/operator.h"
#include "src/xnnpack/params.h"
#include <pthreadpool.h>

static inline size_t compute_output_dimension_with_tf_same_padding(
    size_t input_dimension,
    size_t stride_dimension)
{
  return divide_round_up(input_dimension, stride_dimension);
}

enum xnn_status create_average_pooling2d_nhwc(
    uint32_t input_padding_top,
    uint32_t input_padding_right,
    uint32_t input_padding_bottom,
    uint32_t input_padding_left,
    uint32_t pooling_height,
    uint32_t pooling_width,
    uint32_t stride_height,
    uint32_t stride_width,
    float output_min,
    float output_max,
    uint32_t flags,
    enum xnn_operator_type operator_type,
    xnn_operator_t average_pooling_op)
{
  if ((xnn_params.init_flags & XNN_INIT_FLAG_XNNPACK) == 0) {
    xnn_log_error("failed to create %s operator: XNNPACK is not initialized",
      xnn_operator_type_to_string(operator_type));
    return xnn_status_uninitialized;
  }

  const uint32_t pooling_size = pooling_height * pooling_width;
  if (pooling_size == 0) {
    xnn_log_error(
      "failed to create %s operator with %" PRIu32 "x%" PRIu32 " pooling size: "
      "pooling size dimensions must be non-zero",
      xnn_operator_type_to_string(operator_type), pooling_width, pooling_height);
    return xnn_status_invalid_parameter;
  }

  if (stride_height == 0 || stride_width == 0) {
    xnn_log_error(
      "failed to create %s operator with %" PRIu32 "x%" PRIu32 " stride: stride dimensions must be non-zero",
      xnn_operator_type_to_string(operator_type), stride_width, stride_height);
    return xnn_status_invalid_parameter;
  }

  if (isnan(output_min)) {
    xnn_log_error(
      "failed to create %s operator with NaN output lower bound: lower bound must be non-NaN",
      xnn_operator_type_to_string(operator_type));
    return xnn_status_invalid_parameter;
  }

  if (isnan(output_max)) {
    xnn_log_error(
      "failed to create %s operator with NaN output upper bound: upper bound must be non-NaN",
      xnn_operator_type_to_string(operator_type));
    return xnn_status_invalid_parameter;
  }

  const bool any_padding = (input_padding_left | input_padding_top | input_padding_right | input_padding_bottom) != 0;
  if ((flags & XNN_FLAG_TENSORFLOW_SAME_PADDING) != 0) {
    if (any_padding) {
      xnn_log_error(
        "failed to create %s operator with %" PRIu32 "+%" PRIu32 "x%" PRIu32 "+%" PRIu32" padding: "
        "TensorFlow SAME padding can't be combined with explicit padding specification",
        xnn_operator_type_to_string(operator_type),
        input_padding_top, input_padding_left, input_padding_bottom, input_padding_right);
      return xnn_status_invalid_parameter;
    }
  }

  average_pooling_op->padding_top = input_padding_top;
  average_pooling_op->padding_right = input_padding_right;
  average_pooling_op->padding_bottom = input_padding_bottom;
  average_pooling_op->padding_left = input_padding_left;

  average_pooling_op->kernel_height = pooling_height;
  average_pooling_op->kernel_width = pooling_width;
  average_pooling_op->stride_height = stride_height;
  average_pooling_op->stride_width = stride_width;
  average_pooling_op->dilation_height = 1;
  average_pooling_op->dilation_width = 1;

  average_pooling_op->type = operator_type;

  average_pooling_op->flags = flags;

  return xnn_status_success;
}

enum xnn_status xnn_create_average_pooling2d_nhwc_f16(
    uint32_t input_padding_top,
    uint32_t input_padding_right,
    uint32_t input_padding_bottom,
    uint32_t input_padding_left,
    uint32_t pooling_height,
    uint32_t pooling_width,
    uint32_t stride_height,
    uint32_t stride_width,
    float output_min,
    float output_max,
    uint32_t flags,
    xnn_operator_t* average_pooling_op_out)
{
  xnn_operator_t average_pooling_op = NULL;
  enum xnn_status status = xnn_status_invalid_parameter;

  const xnn_float16 fp16_output_min = xnn_float16_from_float(output_min);
  const xnn_float16 fp16_output_max = xnn_float16_from_float(output_max);
  const float rounded_output_min = xnn_float16_to_float(fp16_output_min);
  const float rounded_output_max = xnn_float16_to_float(fp16_output_max);
  if (rounded_output_min >= rounded_output_max) {
    xnn_log_error(
      "failed to create %s operator with [%.7g, %.7g] output range: lower bound must be below upper bound",
      xnn_operator_type_to_string(xnn_operator_type_average_pooling_nhwc_f16), rounded_output_min, rounded_output_max);
    goto error;
  }

  status = xnn_status_out_of_memory;

  average_pooling_op = xnn_allocate_zero_simd_memory(sizeof(struct xnn_operator));
  if (average_pooling_op == NULL) {
    xnn_log_error(
      "failed to allocate %zu bytes for %s operator descriptor",
      sizeof(struct xnn_operator), xnn_operator_type_to_string(xnn_operator_type_average_pooling_nhwc_f16));
    goto error;
  }

  status = create_average_pooling2d_nhwc(input_padding_top, input_padding_right, input_padding_bottom, input_padding_left,
                                         pooling_height, pooling_width, stride_height, stride_width,
                                         output_min, output_max, flags,
                                         xnn_operator_type_average_pooling_nhwc_f16,
                                         average_pooling_op);
  if (status != xnn_status_success) {
    goto error;
  }
  status = xnn_status_unsupported_hardware;

  const struct xnn_avgpool_config* avgpool_config = xnn_init_f16_avgpool_config();
  if (avgpool_config == NULL) {
    xnn_log_error("failed to create %s operator: unsupported hardware configuration",
                  xnn_operator_type_to_string(xnn_operator_type_average_pooling_nhwc_f16));
    goto error;
  }
  average_pooling_op->avgpool_config = avgpool_config;

  const uint32_t pooling_size = pooling_height * pooling_width;
  avgpool_config->init.f16(&average_pooling_op->params.f16_scaleminmax,
    xnn_float16_from_float(1.0f / (float) (int32_t) pooling_size), fp16_output_min, fp16_output_max);
  const bool tf_same_padding = (flags & XNN_FLAG_TENSORFLOW_SAME_PADDING) != 0;
  const bool any_padding = (input_padding_left | input_padding_top | input_padding_right | input_padding_bottom) != 0;
  if (any_padding || tf_same_padding) {
    // pavgpool does not include padding (zero) elements when calculating the average.
    average_pooling_op->ukernel.type = xnn_microkernel_type_pixelwise_average_pooling;
  } else {
    // avgpool includes padding elements when calculating the average.
    average_pooling_op->ukernel.type = xnn_microkernel_type_average_pooling;
  }
  average_pooling_op->flags = flags;

  *average_pooling_op_out = average_pooling_op;
  return xnn_status_success;

error:
  xnn_delete_operator(average_pooling_op);
  return status;
}

enum xnn_status xnn_create_average_pooling2d_nhwc_f32(
    uint32_t input_padding_top,
    uint32_t input_padding_right,
    uint32_t input_padding_bottom,
    uint32_t input_padding_left,
    uint32_t pooling_height,
    uint32_t pooling_width,
    uint32_t stride_height,
    uint32_t stride_width,
    float output_min,
    float output_max,
    uint32_t flags,
    xnn_operator_t* average_pooling_op_out)
{
  xnn_operator_t average_pooling_op = NULL;
  enum xnn_status status = xnn_status_out_of_memory;

  average_pooling_op = xnn_allocate_zero_simd_memory(sizeof(struct xnn_operator));
  if (average_pooling_op == NULL) {
    xnn_log_error(
      "failed to allocate %zu bytes for %s operator descriptor",
      sizeof(struct xnn_operator), xnn_operator_type_to_string(xnn_operator_type_average_pooling_nhwc_f32));
    goto error;
  }

  status = create_average_pooling2d_nhwc(input_padding_top, input_padding_right, input_padding_bottom, input_padding_left,
                                         pooling_height, pooling_width, stride_height, stride_width,
                                         output_min, output_max, flags,
                                         xnn_operator_type_average_pooling_nhwc_f32,
                                         average_pooling_op);
  if (status != xnn_status_success) {
    goto error;
  }
  const struct xnn_avgpool_config* avgpool_config = xnn_init_f32_avgpool_config();
  status = xnn_status_unsupported_hardware;
  if (avgpool_config == NULL) {
    xnn_log_error("failed to create %s operator: unsupported hardware configuration",
                  xnn_operator_type_to_string(xnn_operator_type_average_pooling_nhwc_f32));
    goto error;
  }
  average_pooling_op->avgpool_config = avgpool_config;

  const uint32_t pooling_size = pooling_height * pooling_width;
  avgpool_config->init.f32(&average_pooling_op->params.f32_scaleminmax,
    1.0f / (float) (int32_t) pooling_size, output_min, output_max);
  const bool tf_same_padding = (flags & XNN_FLAG_TENSORFLOW_SAME_PADDING) != 0;
  const bool any_padding = (input_padding_left | input_padding_top | input_padding_right | input_padding_bottom);
  if (any_padding || tf_same_padding) {
    // pavgpool does not include padding (zero) elements when calculating the average.
    average_pooling_op->ukernel.type = xnn_microkernel_type_pixelwise_average_pooling;
  } else {
    // avgpool includes padding elements when calculating the average.
    average_pooling_op->ukernel.type = xnn_microkernel_type_average_pooling;
  }

  *average_pooling_op_out = average_pooling_op;
  return xnn_status_success;

error:
  xnn_delete_operator(average_pooling_op);
  return status;
}

static enum xnn_status reshape_average_pooling2d(
  xnn_operator_t average_pooling_op,
  size_t batch_size,
  size_t input_height,
  size_t input_width,
  size_t channels,
  size_t input_pixel_stride,
  size_t output_pixel_stride,
  uint32_t log2_data_element_size,
  uint32_t log2_weight_element_size,
  uint32_t log2_accumulator_element_size,
  xnn_indirection_init_pavgpool2d_fn indirection_init_pavgpool2d,
  const struct xnn_avgpool_config avgpool[restrict XNN_MIN_ELEMENTS(1)],
  const void* params,
  size_t params_size,
  size_t* output_height_out,
  size_t* output_width_out,
  pthreadpool_t threadpool,
  enum xnn_operator_type operator_type,
  bool is_pixelwise)
{
  if (channels == 0) {
    xnn_log_error(
      "failed to create %s operator with %zu channels: number of channels must be non-zero",
      xnn_operator_type_to_string(operator_type), channels);
    return xnn_status_invalid_parameter;
  }
  if (input_pixel_stride < channels) {
    xnn_log_error(
      "failed to create %s operator with input pixel stride of %zu: "
      "stride must be at least as large as the number of channels (%zu)",
      xnn_operator_type_to_string(operator_type), input_pixel_stride, channels);
    return xnn_status_invalid_parameter;
  }

  if (output_pixel_stride < channels) {
    xnn_log_error(
      "failed to create %s operator with output pixel stride of %zu: "
      "stride must be at least as large as the number of channels (%zu)",
      xnn_operator_type_to_string(operator_type), output_pixel_stride, channels);
    return xnn_status_invalid_parameter;
  }

  const size_t zero_bytes = (channels << log2_data_element_size) + XNN_EXTRA_BYTES;

  const size_t last_input_channels = average_pooling_op->last_input_channels;
  const size_t last_input_height = average_pooling_op->last_input_height;
  const size_t last_input_width = average_pooling_op->last_input_width;

  const bool input_size_changed =  (input_height != last_input_height || input_width != last_input_width || channels != last_input_channels);
  void* zero_buffer = average_pooling_op->zero_buffer;
  if (input_size_changed) {
    xnn_release_simd_memory(zero_buffer);
    zero_buffer =
      (void*) xnn_allocate_zero_simd_memory(zero_bytes);
    if (zero_buffer == NULL) {
      xnn_log_error(
          "failed to allocate %zu bytes for %s operator zero padding",
          zero_bytes, xnn_operator_type_to_string(operator_type));
      return xnn_status_out_of_memory;
    }
    average_pooling_op->zero_buffer = zero_buffer;
  }
  average_pooling_op->channels = channels;
  average_pooling_op->input_pixel_stride = input_pixel_stride;
  average_pooling_op->output_pixel_stride = output_pixel_stride;

  assert(!is_pixelwise || (indirection_init_pavgpool2d != NULL));

  average_pooling_op->state = xnn_run_state_invalid;

  if ((xnn_params.init_flags & XNN_INIT_FLAG_XNNPACK) == 0) {
    xnn_log_error("failed to reshape %s operator: XNNPACK is not initialized",
                  xnn_operator_type_to_string_v2(average_pooling_op));
    return xnn_status_uninitialized;
  }

  if (input_width == 0 || input_height == 0) {
    xnn_log_error(
        "failed to reshape %s operator with %zux%zu input: input dimensions "
        "must be non-zero",
        xnn_operator_type_to_string_v2(average_pooling_op), input_width,
        input_height);
    return xnn_status_invalid_parameter;
  }

  if (batch_size == 0) {
    average_pooling_op->state = xnn_run_state_skip;
    return xnn_status_success;
  }

  average_pooling_op->input_height = input_height;
  average_pooling_op->input_width = input_width;

  const bool tf_same_padding = (average_pooling_op->flags & XNN_FLAG_TENSORFLOW_SAME_PADDING) != 0;
  if (tf_same_padding) {
    average_pooling_op->output_height = compute_output_dimension_with_tf_same_padding(
        input_height, average_pooling_op->stride_height);
    average_pooling_op->output_width = compute_output_dimension_with_tf_same_padding(
        input_width, average_pooling_op->stride_width);

    const uint32_t kernel_height = average_pooling_op->kernel_height;
    const uint32_t kernel_width = average_pooling_op->kernel_width;
    const uint32_t total_padding_height =
      (average_pooling_op->output_height - 1) * average_pooling_op->stride_height + kernel_height - input_height;
    const uint32_t total_padding_width =
      (average_pooling_op->output_width - 1) * average_pooling_op->stride_width + kernel_width - input_width;
    average_pooling_op->padding_top = total_padding_height / 2;
    average_pooling_op->padding_left = total_padding_width / 2;
    average_pooling_op->padding_bottom = total_padding_height - average_pooling_op->padding_top;
    average_pooling_op->padding_right = total_padding_width - average_pooling_op->padding_left;
  } else {
    average_pooling_op->output_height = xnn_compute_convolution_output_dimension(
        average_pooling_op->padding_top + input_height + average_pooling_op->padding_bottom,
        average_pooling_op->kernel_height,
        1,
        average_pooling_op->stride_height);
    average_pooling_op->output_width = xnn_compute_convolution_output_dimension(
        average_pooling_op->padding_left + input_width + average_pooling_op->padding_right,
        average_pooling_op->kernel_width,
        1,
        average_pooling_op->stride_width);
  }

  if (output_height_out != NULL) {
    *output_height_out = average_pooling_op->output_height;
  }
  if (output_width_out != NULL) {
    *output_width_out = average_pooling_op->output_width;
  }

  const size_t output_height = average_pooling_op->output_height;
  const size_t output_width = average_pooling_op->output_width;
  const size_t pooling_height = average_pooling_op->kernel_height;
  const size_t pooling_width = average_pooling_op->kernel_width;
  const size_t pooling_size = pooling_height * pooling_width;

  const size_t step_width = min(average_pooling_op->stride_width, pooling_width);
  const size_t step_height = pooling_size + (output_width - 1) * step_width * pooling_height;

  const size_t indirect_top_height = divide_round_up(average_pooling_op->padding_top, average_pooling_op->stride_height);
  const size_t indirect_bot_height = divide_round_up(average_pooling_op->padding_bottom, average_pooling_op->stride_height);

  if (input_size_changed) {
    const size_t indirection_buffer_output_height = (indirect_top_height + indirect_bot_height + 1);
    const size_t indirection_buffer_size = sizeof(void*) * ((pooling_size - 1) + indirection_buffer_output_height * step_height);

    const void** indirection_buffer =
      (const void**) xnn_reallocate_memory(average_pooling_op->indirection_buffer, indirection_buffer_size);
    if (indirection_buffer == NULL) {
      xnn_log_error(
          "failed to allocate %zu bytes for %s operator indirection buffer",
          indirection_buffer_size,
          xnn_operator_type_to_string_v2(average_pooling_op));
      return xnn_status_out_of_memory;
    }
    average_pooling_op->indirection_buffer = indirection_buffer;
    xnn_log_debug("allocated %zu bytes for indirection buffer in %s operator",
                  indirection_buffer_size,
                  xnn_operator_type_to_string_v2(average_pooling_op));

    // Set a dummy input first, the actual input offset is calculated in setup when we have the input pointer.
    // This offset must be aligned properly because inputs and input offsets need to be aligned.
    average_pooling_op->input = (void*) ((uintptr_t) average_pooling_op->zero_buffer + XNN_ALLOCATION_ALIGNMENT);
    average_pooling_op->last_input = average_pooling_op->input;
    xnn_indirection_init_dwconv2d_compressed(
      /*output_y_start=*/0, /*output_y_end=*/average_pooling_op->output_height,
      average_pooling_op->indirection_buffer,
      average_pooling_op->input,
      average_pooling_op->input_pixel_stride << log2_data_element_size,
      average_pooling_op->zero_buffer,
      average_pooling_op->input_height, average_pooling_op->input_width,
      average_pooling_op->output_height, average_pooling_op->output_width,
      average_pooling_op->kernel_height, average_pooling_op->kernel_width,
      average_pooling_op->stride_height, average_pooling_op->stride_width,
      average_pooling_op->dilation_height, average_pooling_op->dilation_width,
      average_pooling_op->padding_top, average_pooling_op->padding_left,
      step_height, step_width,
      indirect_top_height, indirect_bot_height,
      /*primary_tile=*/0);

    average_pooling_op->last_input_height = input_height;
    average_pooling_op->last_input_width = input_width;
    average_pooling_op->last_input_channels = channels;
  }

  const size_t indirect_input_height_stride = step_height * sizeof(void*);
  const size_t output_width_stride = average_pooling_op->output_pixel_stride << log2_data_element_size;
  const size_t output_height_stride = output_width * output_width_stride;

  if (is_pixelwise) {
    assert(indirection_init_pavgpool2d != NULL);
    average_pooling_op->ukernel.subtype = xnn_microkernel_type_pixelwise_average_pooling;

    if (input_size_changed) {
      const size_t pixelwise_buffer_size = (output_height * output_width) << log2_weight_element_size;
      void* pixelwise_buffer = xnn_reallocate_memory(average_pooling_op->pixelwise_buffer, pixelwise_buffer_size);
      if (pixelwise_buffer == NULL) {
        xnn_log_error(
            "failed to allocate %zu bytes for %s operator pixelwise buffer",
            pixelwise_buffer_size,
            xnn_operator_type_to_string_v2(average_pooling_op));
        return xnn_status_out_of_memory;
      }
      average_pooling_op->pixelwise_buffer = pixelwise_buffer;
      xnn_log_debug("allocated %zu bytes for pixelwise buffer in %s operator",
                    pixelwise_buffer_size,
                    xnn_operator_type_to_string_v2(average_pooling_op));

      indirection_init_pavgpool2d(
        input_height, input_width,
        output_height, output_width,
        average_pooling_op->kernel_height, average_pooling_op->kernel_width,
        average_pooling_op->stride_height, average_pooling_op->stride_width,
        average_pooling_op->padding_top, average_pooling_op->padding_left,
        pixelwise_buffer);
    }
  } else {
    // Not pixelwise.
    average_pooling_op->ukernel.subtype = xnn_microkernel_type_average_pooling;
  }

  average_pooling_op->context.average_pooling = (struct average_pooling_context) {
    .indirect_input = average_pooling_op->indirection_buffer,
    .indirect_input_height_stride = indirect_input_height_stride,
    .indirect_top_height = indirect_top_height,
    .indirect_bot_start = average_pooling_op->output_height - indirect_bot_height,
    .input_batch_stride = input_height * input_width * average_pooling_op->input_pixel_stride << log2_data_element_size,
    .input_y_stride =
        average_pooling_op->stride_height * input_width * average_pooling_op->input_pixel_stride
        << log2_data_element_size,
    .pixelwise_buffer = average_pooling_op->pixelwise_buffer,
    .pixelwise_buffer_height_stride = output_width << log2_data_element_size,
    .output_batch_stride = output_height * output_height_stride,
    .output_height_stride = output_height_stride,
    .output_width = output_width,
    .pooling_size = pooling_size,
    .channels = channels,
    .zero = average_pooling_op->zero_buffer,
    .input_increment = (pooling_height * step_width) * sizeof(void*),
    .output_increment = output_width_stride,
  };
  memcpy(&average_pooling_op->context.average_pooling.params, params, params_size);
  average_pooling_op->context.average_pooling.ukernel = avgpool->ukernel;
  average_pooling_op->compute[0].type = xnn_parallelization_type_2d;
  average_pooling_op->compute[0].task_2d = (pthreadpool_task_2d_t) xnn_compute_average_pooling;
  average_pooling_op->compute[0].range[0] = batch_size;
  average_pooling_op->compute[0].range[1] = output_height;
  average_pooling_op->state = xnn_run_state_needs_setup;

  return xnn_status_success;
}

enum xnn_status xnn_reshape_average_pooling2d_nhwc_f16(
  xnn_operator_t average_pooling_op,
  size_t batch_size,
  size_t input_height,
  size_t input_width,
  size_t channels,
  size_t input_pixel_stride,
  size_t output_pixel_stride,
  size_t* output_height_out,
  size_t* output_width_out,
  pthreadpool_t threadpool)
{
  if (average_pooling_op->type != xnn_operator_type_average_pooling_nhwc_f16) {
    xnn_log_error(
        "failed to reshape operator: operator type mismatch (expected %s, got "
        "%s)",
        xnn_operator_type_to_string(xnn_operator_type_average_pooling_nhwc_f16),
        xnn_operator_type_to_string_v2(average_pooling_op));
    return xnn_status_invalid_parameter;
  }

  assert(average_pooling_op->ukernel.type == xnn_microkernel_type_average_pooling ||
         average_pooling_op->ukernel.type == xnn_microkernel_type_pixelwise_average_pooling);

  const void* pooling_params = &average_pooling_op->params.f16_scaleminmax;
  size_t pooling_params_size = sizeof(average_pooling_op->params.f16_scaleminmax);
  const bool is_pixelwise = average_pooling_op->ukernel.type == xnn_microkernel_type_pixelwise_average_pooling;

  return reshape_average_pooling2d(
    average_pooling_op,
    batch_size, input_height, input_width, channels, input_pixel_stride, output_pixel_stride,
    /*log2_data_element_size=*/XNN_LOG2_SIZEOF_HALF,
    /*log2_weight_element_size=*/XNN_LOG2_SIZEOF_HALF,
    /*log2_accumulator_element_size=*/XNN_LOG2_SIZEOF_HALF,
    (xnn_indirection_init_pavgpool2d_fn) xnn_indirection_init_pavgpool2d_f16,
    average_pooling_op->avgpool_config,
    pooling_params, pooling_params_size,
    output_height_out, output_width_out,
    threadpool,
    xnn_operator_type_average_pooling_nhwc_f16,
    is_pixelwise);
}

enum xnn_status xnn_reshape_average_pooling2d_nhwc_f32(
  xnn_operator_t average_pooling_op,
  size_t batch_size,
  size_t input_height,
  size_t input_width,
  size_t channels,
  size_t input_pixel_stride,
  size_t output_pixel_stride,
  size_t* output_height_out,
  size_t* output_width_out,
  pthreadpool_t threadpool)
{
  if (average_pooling_op->type != xnn_operator_type_average_pooling_nhwc_f32) {
    xnn_log_error(
        "failed to reshape operator: operator type mismatch (expected %s, got "
        "%s)",
        xnn_operator_type_to_string(xnn_operator_type_average_pooling_nhwc_f32),
        xnn_operator_type_to_string_v2(average_pooling_op));
    return xnn_status_invalid_parameter;
  }

  assert(average_pooling_op->ukernel.type == xnn_microkernel_type_average_pooling ||
         average_pooling_op->ukernel.type == xnn_microkernel_type_pixelwise_average_pooling);

  const void* pooling_params = &average_pooling_op->params.f32_scaleminmax;
  size_t pooling_params_size = sizeof(average_pooling_op->params.f32_scaleminmax);
  const bool is_pixelwise = average_pooling_op->ukernel.type == xnn_microkernel_type_pixelwise_average_pooling;

  return reshape_average_pooling2d(
    average_pooling_op,
    batch_size, input_height, input_width, channels, input_pixel_stride, output_pixel_stride,
    /*log2_data_element_size=*/XNN_LOG2_SIZEOF_FLOAT,
    /*log2_weight_element_size=*/XNN_LOG2_SIZEOF_FLOAT,
    /*log2_accumulator_element_size=*/XNN_LOG2_SIZEOF_FLOAT,
    (xnn_indirection_init_pavgpool2d_fn) xnn_indirection_init_pavgpool2d_f32,
    average_pooling_op->avgpool_config,
    pooling_params, pooling_params_size,
    output_height_out, output_width_out,
    threadpool,
    xnn_operator_type_average_pooling_nhwc_f32,
    is_pixelwise);
}

static enum xnn_status setup_average_pooling2d(
  xnn_operator_t average_pooling_op,
  const void* input,
  void* output)
{
  switch (average_pooling_op->state) {
    case xnn_run_state_skip:
      return xnn_status_success;
    case xnn_run_state_invalid:
      xnn_log_error(
          "failed to setup %s operator: operator has not been reshaped yet",
          xnn_operator_type_to_string_v2(average_pooling_op));
      return xnn_status_invalid_state;
    case xnn_run_state_needs_setup:
      // Operator has been reshaped, but not setup, continue with setup.
    case xnn_run_state_ready:
      // Operator has been reshaped, and we are setting up with different pointers.
      break;
  }

  average_pooling_op->output = output;

  average_pooling_op->context.average_pooling.input_offset =
    (size_t) ((uintptr_t) input - (uintptr_t) average_pooling_op->last_input);
  average_pooling_op->context.average_pooling.output = output;
  average_pooling_op->state = xnn_run_state_ready;

  return xnn_status_success;
}

enum xnn_status xnn_setup_average_pooling2d_nhwc_f16(
    xnn_operator_t average_pooling_op,
    const void* input,
    void* output)
{
  if (average_pooling_op->type != xnn_operator_type_average_pooling_nhwc_f16) {
    xnn_log_error(
        "failed to setup operator: operator type mismatch (expected %s, got "
        "%s)",
        xnn_operator_type_to_string(xnn_operator_type_average_pooling_nhwc_f16),
        xnn_operator_type_to_string_v2(average_pooling_op));
    return xnn_status_invalid_parameter;
  }

  assert(average_pooling_op->ukernel.type == xnn_microkernel_type_average_pooling ||
         average_pooling_op->ukernel.type == xnn_microkernel_type_pixelwise_average_pooling);

  return setup_average_pooling2d(
    average_pooling_op,
    input, output);
}

enum xnn_status xnn_setup_average_pooling2d_nhwc_f32(
    xnn_operator_t average_pooling_op,
    const float* input,
    float* output)
{
  if (average_pooling_op->type != xnn_operator_type_average_pooling_nhwc_f32) {
    xnn_log_error(
        "failed to setup operator: operator type mismatch (expected %s, got "
        "%s)",
        xnn_operator_type_to_string(xnn_operator_type_average_pooling_nhwc_f32),
        xnn_operator_type_to_string_v2(average_pooling_op));
    return xnn_status_invalid_parameter;
  }

  assert(average_pooling_op->ukernel.type == xnn_microkernel_type_average_pooling ||
         average_pooling_op->ukernel.type == xnn_microkernel_type_pixelwise_average_pooling);

  return setup_average_pooling2d(
    average_pooling_op,
    input, output);
}
