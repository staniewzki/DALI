// Copyright (c) 2022, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DALI_IMGCODEC_DECODERS_NVJPEG2K_CONVERT_GPU_H_
#define DALI_IMGCODEC_DECODERS_NVJPEG2K_CONVERT_GPU_H_

#include <driver_types.h>
#include "dali/core/tensor_shape.h"

namespace dali {
namespace imgcodec {

template<class DataType>
void AdjustBitdepthImpl(DataType *in, DataType *out, int in_bpp, cudaStream_t stream,
                        const TensorShape<3> &shape);

}  // namespace imgcodec
}  // namespace dali

#endif  // DALI_IMGCODEC_DECODERS_NVJPEG2K_CONVERT_GPU_H_
