/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <cstdint>

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/kernels/kernel_runner.h"
#include "tensorflow/lite/micro/testing/micro_test.h"
#include "tensorflow/lite/micro/testing/test_utils.h"

namespace tflite {
namespace testing {
namespace {

void TestAveragePoolingFloat(std::initializer_list<int> input_dims_data,
                             std::initializer_list<float> input_data,
                             const int filter_height, const int filter_width,
                             const int stride_height, const int stride_width,
                             std::initializer_list<float> expected_output_data,
                             std::initializer_list<int> output_dims_data,
                             TfLitePadding padding,
                             TfLiteFusedActivation activation,
                             float* output_data) {
  TfLiteIntArray* input_dims = IntArrayFromInitializer(input_dims_data);
  TfLiteIntArray* output_dims = IntArrayFromInitializer(output_dims_data);
  const int output_dims_count = ElementCount(*output_dims);

  constexpr int inputs_size = 1;
  constexpr int outputs_size = 1;
  constexpr int tensors_size = inputs_size + outputs_size;
  TfLiteTensor tensors[tensors_size] = {
      CreateFloatTensor(input_data, input_dims),
      CreateFloatTensor(output_data, output_dims),
  };

  int inputs_array_data[] = {1, 0};
  TfLiteIntArray* inputs_array = IntArrayFromInts(inputs_array_data);
  int outputs_array_data[] = {1, 1};
  TfLiteIntArray* outputs_array = IntArrayFromInts(outputs_array_data);

  TfLitePoolParams builtin_data = {padding,
                                   stride_width,
                                   stride_height,
                                   filter_width,
                                   filter_height,
                                   activation,
                                   {}};

  const TfLiteRegistration registration =
      tflite::ops::micro::Register_AVERAGE_POOL_2D();
  micro::KernelRunner runner(
      registration, tensors, tensors_size, inputs_array, outputs_array,
      reinterpret_cast<void*>(&builtin_data), micro_test::reporter);

  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.InitAndPrepare());
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.Invoke());

  for (int i = 0; i < output_dims_count; ++i) {
    TF_LITE_MICRO_EXPECT_NEAR(expected_output_data.begin()[i], output_data[i],
                              1e-5f);
  }
}

template <typename T>
void TestAveragePoolingQuantized(
    std::initializer_list<int> input_dims_data,
    std::initializer_list<T> input_data, const float input_min,
    const float input_max, const int filter_height, const int filter_width,
    const int stride_height, const int stride_width,
    std::initializer_list<T> expected_output_data,
    std::initializer_list<int> output_dims_data, float output_min,
    float output_max, TfLitePadding padding, TfLiteFusedActivation activation,
    T* output_data) {
  static_assert(sizeof(T) == 1, "Only int8_t/uint8_t data types allowed.");

  TfLiteIntArray* input_dims = IntArrayFromInitializer(input_dims_data);
  TfLiteIntArray* output_dims = IntArrayFromInitializer(output_dims_data);
  const int output_dims_count = ElementCount(*output_dims);

  constexpr int inputs_size = 1;
  constexpr int outputs_size = 1;
  constexpr int tensors_size = inputs_size + outputs_size;
  TfLiteTensor tensors[tensors_size] = {
      CreateQuantizedTensor(input_data, input_dims, input_min, input_max),
      CreateQuantizedTensor(output_data, output_dims, output_min, output_max),
  };

  int inputs_array_data[] = {1, 0};
  TfLiteIntArray* inputs_array = IntArrayFromInts(inputs_array_data);
  int outputs_array_data[] = {1, 1};
  TfLiteIntArray* outputs_array = IntArrayFromInts(outputs_array_data);

  TfLitePoolParams builtin_data = {padding,
                                   stride_width,
                                   stride_height,
                                   filter_width,
                                   filter_height,
                                   activation,
                                   {}};

  const TfLiteRegistration registration =
      tflite::ops::micro::Register_AVERAGE_POOL_2D();
  micro::KernelRunner runner(
      registration, tensors, tensors_size, inputs_array, outputs_array,
      reinterpret_cast<void*>(&builtin_data), micro_test::reporter);

  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.InitAndPrepare());
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.Invoke());

  for (int i = 0; i < output_dims_count; ++i) {
    TF_LITE_MICRO_EXPECT_NEAR(expected_output_data.begin()[i], output_data[i],
                              1e-5f);
  }
}

void TestMaxPoolFloat(std::initializer_list<int> input_dims_data,
                      std::initializer_list<float> input_data, int filter_width,
                      int filter_height, int stride_width, int stride_height,
                      std::initializer_list<float> expected_output_data,
                      std::initializer_list<int> output_dims_data,
                      TfLitePadding padding, TfLiteFusedActivation activation,
                      float* output_data) {
  TfLiteIntArray* input_dims = IntArrayFromInitializer(input_dims_data);
  TfLiteIntArray* output_dims = IntArrayFromInitializer(output_dims_data);
  const int output_dims_count = ElementCount(*output_dims);

  constexpr int inputs_size = 1;
  constexpr int outputs_size = 1;
  constexpr int tensors_size = inputs_size + outputs_size;
  TfLiteTensor tensors[tensors_size] = {
      CreateFloatTensor(input_data, input_dims),
      CreateFloatTensor(output_data, output_dims),
  };

  int inputs_array_data[] = {1, 0};
  TfLiteIntArray* inputs_array = IntArrayFromInts(inputs_array_data);
  int outputs_array_data[] = {1, 1};
  TfLiteIntArray* outputs_array = IntArrayFromInts(outputs_array_data);

  TfLitePoolParams builtin_data = {padding,
                                   stride_width,
                                   stride_height,
                                   filter_width,
                                   filter_height,
                                   activation,
                                   {}};

  const TfLiteRegistration registration =
      tflite::ops::micro::Register_MAX_POOL_2D();
  micro::KernelRunner runner(
      registration, tensors, tensors_size, inputs_array, outputs_array,
      reinterpret_cast<void*>(&builtin_data), micro_test::reporter);

  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.InitAndPrepare());
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.Invoke());

  for (int i = 0; i < output_dims_count; ++i) {
    TF_LITE_MICRO_EXPECT_NEAR(expected_output_data.begin()[i], output_data[i],
                              1e-5f);
  }
}

template <typename T>
void TestMaxPoolQuantized(std::initializer_list<int> input_dims_data,
                          std::initializer_list<T> input_data, float input_min,
                          float input_max, int filter_width, int filter_height,
                          int stride_width, int stride_height,
                          std::initializer_list<T> expected_output_data,
                          float output_min, float output_max,
                          std::initializer_list<int> output_dims_data,
                          TfLitePadding padding,
                          TfLiteFusedActivation activation, T* output_data) {
  static_assert(sizeof(T) == 1, "Only int8_t/uint8_t data types allowed.");

  TfLiteIntArray* input_dims = IntArrayFromInitializer(input_dims_data);
  TfLiteIntArray* output_dims = IntArrayFromInitializer(output_dims_data);
  const int output_dims_count = ElementCount(*output_dims);

  constexpr int inputs_size = 1;
  constexpr int outputs_size = 1;
  constexpr int tensors_size = inputs_size + outputs_size;
  TfLiteTensor tensors[tensors_size] = {
      CreateQuantizedTensor(input_data, input_dims, input_min, input_max),
      CreateQuantizedTensor(output_data, output_dims, output_min, output_max),
  };

  int inputs_array_data[] = {1, 0};
  TfLiteIntArray* inputs_array = IntArrayFromInts(inputs_array_data);
  int outputs_array_data[] = {1, 1};
  TfLiteIntArray* outputs_array = IntArrayFromInts(outputs_array_data);

  TfLitePoolParams builtin_data = {padding,
                                   stride_width,
                                   stride_height,
                                   filter_width,
                                   filter_height,
                                   activation,
                                   {}};

  const TfLiteRegistration registration =
      tflite::ops::micro::Register_MAX_POOL_2D();
  micro::KernelRunner runner(
      registration, tensors, tensors_size, inputs_array, outputs_array,
      reinterpret_cast<void*>(&builtin_data), micro_test::reporter);

  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.InitAndPrepare());
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.Invoke());

  for (int i = 0; i < output_dims_count; ++i) {
    TF_LITE_MICRO_EXPECT_EQ(expected_output_data.begin()[i], output_data[i]);
  }
}

}  // namespace

}  // namespace testing
}  // namespace tflite

TF_LITE_MICRO_TESTS_BEGIN

TF_LITE_MICRO_TEST(SimpleAveragePoolTestFloat) {
  float output_data[2];
  tflite::testing::TestAveragePoolingFloat({4, 1, 2, 4, 1},  // Input shape
                                           {                 // Input values
                                            0., 6., 2., 4., 3., 2., 10., 7.},
                                           2, 2,  // filter width, filter height
                                           2, 2,  // stride width, stride height
                                           {
                                               // Output values
                                               2.75,
                                               5.75,
                                           },
                                           {4, 1, 1, 2, 1},  // Output shape
                                           kTfLitePaddingValid, kTfLiteActNone,
                                           output_data);
}

TF_LITE_MICRO_TEST(SimpleAveragePoolTestUint8) {
  using tflite::testing::F2Q;

  const float input_min = -15.9375;
  const float input_max = 15.9375;
  const float output_min = -15.9375;
  const float output_max = 15.9375;
  uint8_t output_data[2];
  tflite::testing::TestAveragePoolingQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2Q(0., input_min, input_max),
          F2Q(-6., input_min, input_max),
          F2Q(2., input_min, input_max),
          F2Q(4., input_min, input_max),
          F2Q(3., input_min, input_max),
          F2Q(2., input_min, input_max),
          F2Q(-10., input_min, input_max),
          F2Q(7., input_min, input_max),
      },
      input_min, input_max,  // input quantization range
      2, 2,                  // filter width, filter height
      2, 2,                  // stride width, stride height
      {
          // Output values
          F2Q(0., output_min, output_max),
          F2Q(0.75, output_min, output_max),
      },
      {4, 1, 1, 2, 1},         // Output shape
      output_min, output_max,  // output quantization range
      kTfLitePaddingValid, kTfLiteActRelu, output_data);
}

TF_LITE_MICRO_TEST(SimpleAveragePoolTestInt8PaddingValidStride2ActNone) {
  using tflite::testing::F2QS;

  const float input_min = -15.9375;
  const float input_max = 15.8130;
  const float output_min = -15.9375;
  const float output_max = 15.8130;
  int8_t output_data[2];
  tflite::testing::TestAveragePoolingQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {                 // Input values
       F2QS(0., input_min, input_max), F2QS(-6., input_min, input_max),
       F2QS(2., input_min, input_max), F2QS(4., input_min, input_max),
       F2QS(3., input_min, input_max), F2QS(2., input_min, input_max),
       F2QS(-10., input_min, input_max), F2QS(7., input_min, input_max)},
      input_min, input_max,  // input quantization range
      2, 2,                  // filter height, filter width
      2, 2,                  // stride height, stride width
      {                      // Output values
       F2QS(-0.25, output_min, output_max), F2QS(0.75, output_min, output_max)},
      {4, 1, 1, 2, 1},         // Output shape
      output_min, output_max,  // output quantization range
      kTfLitePaddingValid, kTfLiteActNone, output_data);
}

TF_LITE_MICRO_TEST(SimpleAveragePoolTestInt8PaddingValidStride1Stride2Relu) {
  using tflite::testing::F2QS;

  const float input_min = -15.9375;
  const float input_max = 15.8130;
  const float output_min = -15.9375;
  const float output_max = 15.8130;
  int8_t output_data[3];
  tflite::testing::TestAveragePoolingQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {                 // Input values
       F2QS(0., input_min, input_max), F2QS(-6., input_min, input_max),
       F2QS(2., input_min, input_max), F2QS(4., input_min, input_max),
       F2QS(3., input_min, input_max), F2QS(2., input_min, input_max),
       F2QS(-10., input_min, input_max), F2QS(7., input_min, input_max)},
      input_min, input_max,  // input quantization range
      2, 2,                  // filter height, filter width
      2, 1,                  // stride height, stride width
      {                      // Output values
       F2QS(0., output_min, output_max), F2QS(0., output_min, output_max),
       F2QS(0.75, output_min, output_max)},
      {4, 1, 1, 3, 1},         // Output shape
      output_min, output_max,  // output quantization range
      kTfLitePaddingValid, kTfLiteActRelu, output_data);
}

TF_LITE_MICRO_TEST(
    SimpleAveragePoolTestInt8PaddingValidStride2Stride1ReluN1To1) {
  using tflite::testing::F2QS;

  const float input_min = -15.9375;
  const float input_max = 15.8130;
  const float output_min = -15.9375;
  const float output_max = 15.8130;
  int8_t output_data[2];
  tflite::testing::TestAveragePoolingQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {                 // Input values
       F2QS(0., input_min, input_max), F2QS(-6., input_min, input_max),
       F2QS(2., input_min, input_max), F2QS(4., input_min, input_max),
       F2QS(3., input_min, input_max), F2QS(2., input_min, input_max),
       F2QS(-10., input_min, input_max), F2QS(7., input_min, input_max)},
      input_min, input_max,  // input quantization range
      2, 2,                  // filter height, filter width
      1, 2,                  // stride height, stride width
      {                      // Output values
       F2QS(-0.25, output_min, output_max), F2QS(0.75, output_min, output_max)},
      {4, 1, 1, 2, 1},         // Output shape
      output_min, output_max,  // output quantization range
      kTfLitePaddingValid, kTfLiteActReluN1To1, output_data);
}

TF_LITE_MICRO_TEST(SimpleAveragePoolTestInt8PaddingValidStride2Relu6) {
  using tflite::testing::F2QS;

  const float input_min = -15.9375;
  const float input_max = 15.8130;
  const float output_min = -15.9375;
  const float output_max = 15.8130;
  int8_t output_data[2];
  tflite::testing::TestAveragePoolingQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {                 // Input values
       F2QS(3., input_min, input_max), F2QS(-6., input_min, input_max),
       F2QS(8., input_min, input_max), F2QS(4., input_min, input_max),
       F2QS(3., input_min, input_max), F2QS(2., input_min, input_max),
       F2QS(10., input_min, input_max), F2QS(7., input_min, input_max)},
      input_min, input_max,  // input quantization range
      2, 2,                  // filter height, filter width
      2, 2,                  // stride height, stride width
      {                      // Output values
       F2QS(0.5, output_min, output_max), F2QS(6., output_min, output_max)},
      {4, 1, 1, 2, 1},         // Output shape
      output_min, output_max,  // output quantization range
      kTfLitePaddingValid, kTfLiteActRelu6, output_data);
}

TF_LITE_MICRO_TEST(SimpleAveragePoolTestInt8PaddingSameStride1ActNone) {
  using tflite::testing::F2QS;

  const float input_min = -15.9375;
  const float input_max = 15.8130;
  const float output_min = -15.9375;
  const float output_max = 15.8130;
  int8_t output_data[8];
  tflite::testing::TestAveragePoolingQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {                 // Input values
       F2QS(3., input_min, input_max), F2QS(-6., input_min, input_max),
       F2QS(8., input_min, input_max), F2QS(4., input_min, input_max),
       F2QS(3., input_min, input_max), F2QS(2., input_min, input_max),
       F2QS(10., input_min, input_max), F2QS(7., input_min, input_max)},
      input_min, input_max,  // input quantization range
      2, 2,                  // filter height, filter width
      1, 1,                  // stride height, stride width
      {                      // Output values
       F2QS(0.5, output_min, output_max), F2QS(3.5, output_min, output_max),
       F2QS(7.25, output_min, output_max), F2QS(5.5, output_min, output_max),
       F2QS(2.5, output_min, output_max), F2QS(6., output_min, output_max),
       F2QS(8.5, output_min, output_max), F2QS(7., output_min, output_max)},
      {4, 1, 2, 4, 1},         // Output shape
      output_min, output_max,  // output quantization range
      kTfLitePaddingSame, kTfLiteActNone, output_data);
}

TF_LITE_MICRO_TEST(SimpleMaxPoolTestFloat) {
  float output_data[2];
  tflite::testing::TestMaxPoolFloat({4, 1, 2, 4, 1},  // Input shape
                                    {                 // Input values
                                     0, 6, 2, 4, 3, 2, 10, 7},
                                    2, 2,  // filter width, filter height
                                    2, 2,  // stride width, stride height
                                    {
                                        // Output values
                                        6,
                                        10,
                                    },
                                    {4, 1, 1, 2, 1},  // Output shape
                                    kTfLitePaddingValid, kTfLiteActNone,
                                    output_data);
}

TF_LITE_MICRO_TEST(SimpleMaxPoolTestFloatRelu) {
  float output_data[2];
  tflite::testing::TestMaxPoolFloat({4, 1, 2, 4, 1},  // Input shape
                                    {
                                        // Input values
                                        -1, -6, 2, 4,     //
                                        -3, -2, 10.5, 7,  //
                                    },
                                    2, 2,  // filter width, filter height
                                    2, 2,  // stride width, stride height
                                    {
                                        // Output values
                                        0.0,
                                        10.5,
                                    },
                                    {4, 1, 1, 2, 1},  // Output shape
                                    kTfLitePaddingValid, kTfLiteActRelu,
                                    output_data);
}

TF_LITE_MICRO_TEST(SimpleMaxPoolTestFloatReluN1To1) {
  float output_data[2];
  tflite::testing::TestMaxPoolFloat({4, 1, 2, 4, 1},  // Input shape
                                    {
                                        // Input values
                                        -2.75, -6, 0.2, 0.4,  //
                                        -3, -2, -0.3, 0.7,    //
                                    },
                                    2, 2,  // filter width, filter height
                                    2, 2,  // stride width, stride height
                                    {
                                        // Output values
                                        -1.0,
                                        0.7,
                                    },
                                    {4, 1, 1, 2, 1},  // Output shape
                                    kTfLitePaddingValid, kTfLiteActReluN1To1,
                                    output_data);

  tflite::testing::TestMaxPoolFloat({4, 1, 2, 4, 1},  // Input shape
                                    {
                                        // Input values
                                        -2.75, -6, -2, -4,  //
                                        -3, -2, 10, -7,     //
                                    },
                                    2, 2,  // filter width, filter height
                                    2, 2,  // stride width, stride height
                                    {
                                        // Output values
                                        -1.0,
                                        1.0,
                                    },
                                    {4, 1, 1, 2, 1},  // Output shape
                                    kTfLitePaddingValid, kTfLiteActReluN1To1,
                                    output_data);
}

TF_LITE_MICRO_TEST(SimpleMaxPoolTestFloatRelu6) {
  float output_data[2];
  tflite::testing::TestMaxPoolFloat({4, 1, 2, 4, 1},  // Input shape
                                    {
                                        // Input values
                                        -1.5, -6, 12, 4,  //
                                        -3, -2, 10, 7,    //
                                    },
                                    2, 2,  // filter width, filter height
                                    2, 2,  // stride width, stride height
                                    {
                                        // Output values
                                        0.0,
                                        6.0,
                                    },
                                    {4, 1, 1, 2, 1},  // Output shape
                                    kTfLitePaddingValid, kTfLiteActRelu6,
                                    output_data);

  tflite::testing::TestMaxPoolFloat({4, 1, 2, 4, 1},  // Input shape
                                    {
                                        // Input values
                                        0, 4.5, 12, 4,  //
                                        3, 2, 10, 7,    //
                                    },
                                    2, 2,  // filter width, filter height
                                    2, 2,  // stride width, stride height
                                    {
                                        // Output values
                                        4.5,
                                        6.0,
                                    },
                                    {4, 1, 1, 2, 1},  // Output shape
                                    kTfLitePaddingValid, kTfLiteActRelu6,
                                    output_data);
}

TF_LITE_MICRO_TEST(SimpleMaxPoolTestPaddingSameStride1) {
  float output_data[8];
  tflite::testing::TestMaxPoolFloat({4, 1, 2, 4, 1},  // Input shape
                                    {
                                        // Input values
                                        0, 6, 2, 4,   //
                                        3, 2, 10, 7,  //
                                    },
                                    2, 2,  // filter width, filter height
                                    1, 1,  // stride width, stride height
                                    {
                                        // Output values
                                        6, 10, 10, 7,  //
                                        3, 10, 10, 7,  //
                                    },
                                    {4, 1, 2, 4, 1},  // Output shape
                                    kTfLitePaddingSame, kTfLiteActNone,
                                    output_data);
}

TF_LITE_MICRO_TEST(SimpleMaxPoolTestPaddingValidStride1) {
  float output_data[3];
  tflite::testing::TestMaxPoolFloat({4, 1, 2, 4, 1},  // Input shape
                                    {
                                        // Input values
                                        0, 6, 2, 4,   //
                                        3, 2, 10, 7,  //
                                    },
                                    2, 2,  // filter width, filter height
                                    1, 1,  // stride width, stride height
                                    {
                                        // Output values
                                        6,
                                        10,
                                        10,
                                    },
                                    {4, 1, 1, 3, 1},  // Output shape
                                    kTfLitePaddingValid, kTfLiteActNone,
                                    output_data);
}

TF_LITE_MICRO_TEST(SimpleMaxPoolTestUInt8ActNone) {
  using tflite::testing::F2Q;

  uint8_t output_data[2];
  float input_min = 0;
  float input_max = 15.9375;
  float output_min = 0;
  float output_max = 15.9375;
  int filter_width = 2;
  int filter_height = 2;
  int stride_width = 2;
  int stride_height = 2;
  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2Q(0, input_min, input_max),
          F2Q(6, input_min, input_max),
          F2Q(2, input_min, input_max),
          F2Q(4, input_min, input_max),
          F2Q(3, input_min, input_max),
          F2Q(2, input_min, input_max),
          F2Q(10, input_min, input_max),
          F2Q(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {// Output values
       F2Q(6, output_min, output_max), F2Q(10, output_min, output_max)},
      output_min, output_max, {4, 1, 1, 2, 1},  // Output shape
      kTfLitePaddingValid, kTfLiteActNone, output_data);
}

TF_LITE_MICRO_TEST(MaxPoolTestUInt8ActRelu) {
  using tflite::testing::F2Q;

  uint8_t output_data[2];
  float input_min = -15.9375;
  float input_max = 15.9375;
  float output_min = -15.9375;
  float output_max = 15.9375;
  int filter_width = 2;
  int filter_height = 2;
  int stride_width = 2;
  int stride_height = 2;
  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2Q(-1.5, input_min, input_max),
          F2Q(-6, input_min, input_max),
          F2Q(2, input_min, input_max),
          F2Q(4, input_min, input_max),
          F2Q(-3, input_min, input_max),
          F2Q(-2, input_min, input_max),
          F2Q(10, input_min, input_max),
          F2Q(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {// Output values
       F2Q(0, output_min, output_max), F2Q(10, output_min, output_max)},
      output_min, output_max, {4, 1, 1, 2, 1},  // Output shape
      kTfLitePaddingValid, kTfLiteActRelu, output_data);
}

TF_LITE_MICRO_TEST(MaxPoolTestUInt8ActReluN1To1) {
  using tflite::testing::F2Q;

  uint8_t output_data[2];
  float input_min = -15.9375;
  float input_max = 15.9375;
  float output_min = -15.9375;
  float output_max = 15.9375;
  int filter_width = 2;
  int filter_height = 2;
  int stride_width = 2;
  int stride_height = 2;
  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2Q(-1.7, input_min, input_max),
          F2Q(-6, input_min, input_max),
          F2Q(2, input_min, input_max),
          F2Q(4, input_min, input_max),
          F2Q(-3, input_min, input_max),
          F2Q(-2, input_min, input_max),
          F2Q(-10, input_min, input_max),
          F2Q(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {// Output values
       F2Q(-1.0, output_min, output_max), F2Q(1.0, output_min, output_max)},
      output_min, output_max, {4, 1, 1, 2, 1},  // Output shape
      kTfLitePaddingValid, kTfLiteActReluN1To1, output_data);
}

TF_LITE_MICRO_TEST(MaxPoolTestUInt8ActRelu6) {
  using tflite::testing::F2Q;

  uint8_t output_data[8];
  float input_min = -15.9375;
  float input_max = 15.9375;
  float output_min = -15.9375;
  float output_max = 15.9375;
  int filter_width = 2;
  int filter_height = 2;
  int stride_width = 2;
  int stride_height = 2;
  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2Q(0, input_min, input_max),
          F2Q(-6, input_min, input_max),
          F2Q(12, input_min, input_max),
          F2Q(4, input_min, input_max),
          F2Q(-3, input_min, input_max),
          F2Q(-2, input_min, input_max),
          F2Q(10, input_min, input_max),
          F2Q(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {// Output values
       F2Q(0.0, output_min, output_max), F2Q(6.0, output_min, output_max)},
      output_min, output_max, {4, 1, 1, 2, 1},  // Output shape
      kTfLitePaddingValid, kTfLiteActRelu6, output_data);

  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2Q(0, input_min, input_max),
          F2Q(4.5, input_min, input_max),
          F2Q(12, input_min, input_max),
          F2Q(4, input_min, input_max),
          F2Q(3, input_min, input_max),
          F2Q(2, input_min, input_max),
          F2Q(10, input_min, input_max),
          F2Q(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {// Output values
       F2Q(4.5, output_min, output_max), F2Q(6.0, output_min, output_max)},
      output_min, output_max, {4, 1, 1, 2, 1},  // Output shape
      kTfLitePaddingValid, kTfLiteActRelu6, output_data);
}

TF_LITE_MICRO_TEST(MaxPoolTestUInt8PaddingSameStride1) {
  using tflite::testing::F2Q;

  uint8_t output_data[8];
  float input_min = 0;
  float input_max = 15.9375;
  float output_min = 0;
  float output_max = 15.9375;
  int filter_width = 2;
  int filter_height = 2;
  int stride_width = 1;
  int stride_height = 1;
  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2Q(0, input_min, input_max),
          F2Q(6, input_min, input_max),
          F2Q(2, input_min, input_max),
          F2Q(4, input_min, input_max),
          F2Q(3, input_min, input_max),
          F2Q(2, input_min, input_max),
          F2Q(10, input_min, input_max),
          F2Q(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {
          // Output values
          F2Q(6, output_min, output_max),
          F2Q(10, output_min, output_max),
          F2Q(10, output_min, output_max),
          F2Q(7, output_min, output_max),
          F2Q(3, output_min, output_max),
          F2Q(10, output_min, output_max),
          F2Q(10, output_min, output_max),
          F2Q(7, output_min, output_max),
      },
      output_min, output_max, {4, 1, 2, 4, 1},  // Output shape
      kTfLitePaddingSame, kTfLiteActNone, output_data);
}

TF_LITE_MICRO_TEST(MaxPoolTestUInt8PaddingValidStride1) {
  using tflite::testing::F2Q;

  uint8_t output_data[3];
  float input_min = 0;
  float input_max = 15.9375;
  float output_min = 0;
  float output_max = 15.9375;
  int filter_width = 2;
  int filter_height = 2;
  int stride_width = 1;
  int stride_height = 1;
  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2Q(0, input_min, input_max),
          F2Q(6, input_min, input_max),
          F2Q(2, input_min, input_max),
          F2Q(4, input_min, input_max),
          F2Q(3, input_min, input_max),
          F2Q(2, input_min, input_max),
          F2Q(10, input_min, input_max),
          F2Q(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {
          // Output values
          F2Q(6, output_min, output_max),
          F2Q(10, output_min, output_max),
          F2Q(10, output_min, output_max),
      },
      output_min, output_max, {4, 1, 1, 3, 1},  // Output shape
      kTfLitePaddingValid, kTfLiteActNone, output_data);
}

TF_LITE_MICRO_TEST(SimpleMaxPoolTestInt8ActNone) {
  using tflite::testing::F2QS;

  int8_t output_data[2];
  float input_min = 0;
  float input_max = 15.9375;
  float output_min = 0;
  float output_max = 15.9375;
  int filter_width = 2;
  int filter_height = 2;
  int stride_width = 2;
  int stride_height = 2;
  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2QS(0, input_min, input_max),
          F2QS(6, input_min, input_max),
          F2QS(2, input_min, input_max),
          F2QS(4, input_min, input_max),
          F2QS(3, input_min, input_max),
          F2QS(2, input_min, input_max),
          F2QS(10, input_min, input_max),
          F2QS(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {// Output values
       F2QS(6, output_min, output_max), F2QS(10, output_min, output_max)},
      output_min, output_max, {4, 1, 1, 2, 1},  // Output shape
      kTfLitePaddingValid, kTfLiteActNone, output_data);
}

TF_LITE_MICRO_TEST(MaxPoolTestUInt8ActRelu) {
  using tflite::testing::F2QS;

  int8_t output_data[2];
  float input_min = -15.9375;
  float input_max = 15.9375;
  float output_min = -15.9375;
  float output_max = 15.9375;
  int filter_width = 2;
  int filter_height = 2;
  int stride_width = 2;
  int stride_height = 2;
  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2QS(-1.5, input_min, input_max),
          F2QS(-6, input_min, input_max),
          F2QS(2, input_min, input_max),
          F2QS(4, input_min, input_max),
          F2QS(-3, input_min, input_max),
          F2QS(-2, input_min, input_max),
          F2QS(10, input_min, input_max),
          F2QS(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {// Output values
       F2QS(0, output_min, output_max), F2QS(10, output_min, output_max)},
      output_min, output_max, {4, 1, 1, 2, 1},  // Output shape
      kTfLitePaddingValid, kTfLiteActRelu, output_data);
}

TF_LITE_MICRO_TEST(MaxPoolTestUInt8ActReluN1To1) {
  using tflite::testing::F2QS;

  int8_t output_data[2];
  float input_min = -15.9375;
  float input_max = 15.9375;
  float output_min = -15.9375;
  float output_max = 15.9375;
  int filter_width = 2;
  int filter_height = 2;
  int stride_width = 2;
  int stride_height = 2;
  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2QS(-1.7, input_min, input_max),
          F2QS(-6, input_min, input_max),
          F2QS(2, input_min, input_max),
          F2QS(4, input_min, input_max),
          F2QS(-3, input_min, input_max),
          F2QS(-2, input_min, input_max),
          F2QS(-10, input_min, input_max),
          F2QS(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {// Output values
       F2QS(-1.0, output_min, output_max), F2QS(1.0, output_min, output_max)},
      output_min, output_max, {4, 1, 1, 2, 1},  // Output shape
      kTfLitePaddingValid, kTfLiteActReluN1To1, output_data);
}

TF_LITE_MICRO_TEST(MaxPoolTestUInt8ActRelu6) {
  using tflite::testing::F2QS;

  int8_t output_data[8];
  float input_min = -15.9375;
  float input_max = 15.9375;
  float output_min = -15.9375;
  float output_max = 15.9375;
  int filter_width = 2;
  int filter_height = 2;
  int stride_width = 2;
  int stride_height = 2;
  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2QS(0, input_min, input_max),
          F2QS(-6, input_min, input_max),
          F2QS(12, input_min, input_max),
          F2QS(4, input_min, input_max),
          F2QS(-3, input_min, input_max),
          F2QS(-2, input_min, input_max),
          F2QS(10, input_min, input_max),
          F2QS(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {// Output values
       F2QS(0.0, output_min, output_max), F2QS(6.0, output_min, output_max)},
      output_min, output_max, {4, 1, 1, 2, 1},  // Output shape
      kTfLitePaddingValid, kTfLiteActRelu6, output_data);

  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2QS(0, input_min, input_max),
          F2QS(4.5, input_min, input_max),
          F2QS(12, input_min, input_max),
          F2QS(4, input_min, input_max),
          F2QS(3, input_min, input_max),
          F2QS(2, input_min, input_max),
          F2QS(10, input_min, input_max),
          F2QS(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {// Output values
       F2QS(4.5, output_min, output_max), F2QS(6.0, output_min, output_max)},
      output_min, output_max, {4, 1, 1, 2, 1},  // Output shape
      kTfLitePaddingValid, kTfLiteActRelu6, output_data);
}

TF_LITE_MICRO_TEST(MaxPoolTestUInt8PaddingSameStride1) {
  using tflite::testing::F2QS;

  int8_t output_data[8];
  float input_min = 0;
  float input_max = 15.9375;
  float output_min = 0;
  float output_max = 15.9375;
  int filter_width = 2;
  int filter_height = 2;
  int stride_width = 1;
  int stride_height = 1;
  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2QS(0, input_min, input_max),
          F2QS(6, input_min, input_max),
          F2QS(2, input_min, input_max),
          F2QS(4, input_min, input_max),
          F2QS(3, input_min, input_max),
          F2QS(2, input_min, input_max),
          F2QS(10, input_min, input_max),
          F2QS(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {
          // Output values
          F2QS(6, output_min, output_max),
          F2QS(10, output_min, output_max),
          F2QS(10, output_min, output_max),
          F2QS(7, output_min, output_max),
          F2QS(3, output_min, output_max),
          F2QS(10, output_min, output_max),
          F2QS(10, output_min, output_max),
          F2QS(7, output_min, output_max),
      },
      output_min, output_max, {4, 1, 2, 4, 1},  // Output shape
      kTfLitePaddingSame, kTfLiteActNone, output_data);
}

TF_LITE_MICRO_TEST(MaxPoolTestUInt8PaddingValidStride1) {
  using tflite::testing::F2QS;

  int8_t output_data[3];
  float input_min = 0;
  float input_max = 15.9375;
  float output_min = 0;
  float output_max = 15.9375;
  int filter_width = 2;
  int filter_height = 2;
  int stride_width = 1;
  int stride_height = 1;
  tflite::testing::TestMaxPoolQuantized(
      {4, 1, 2, 4, 1},  // Input shape
      {
          // Input values
          F2QS(0, input_min, input_max),
          F2QS(6, input_min, input_max),
          F2QS(2, input_min, input_max),
          F2QS(4, input_min, input_max),
          F2QS(3, input_min, input_max),
          F2QS(2, input_min, input_max),
          F2QS(10, input_min, input_max),
          F2QS(7, input_min, input_max),
      },
      input_min, input_max, filter_width, filter_height, stride_width,
      stride_height,
      {
          // Output values
          F2QS(6, output_min, output_max),
          F2QS(10, output_min, output_max),
          F2QS(10, output_min, output_max),
      },
      output_min, output_max, {4, 1, 1, 3, 1},  // Output shape
      kTfLitePaddingValid, kTfLiteActNone, output_data);
}

TF_LITE_MICRO_TESTS_END
