// Copyright (c) 2020-2021, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include <nvjpeg.h>
#include <shared_mutex>
#include <atomic>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <utility>
#include <vector>
#include <memory>
#include <unordered_map>
#include "dali/core/cuda_error.h"
#include "dali/core/mm/memory.h"
#include "dali/core/mm/memory_kind.h"
#include "dali/core/mm/malloc_resource.h"
#include "dali/imgcodec/decoders/nvjpeg/nvjpeg_memory.h"
#include "dali/imgcodec/decoders/nvjpeg/memory_pool.h"
#include "dali/pipeline/data/buffer.h"

namespace dali {

namespace imgcodec {

namespace nvjpeg_memory {

template <typename MemoryKind>
void AddBuffer(std::thread::id thread_id, size_t size) {
  NVJpegMem::instance().AddBuffer<MemoryKind>(thread_id, size);
}

void AddHostBuffer(std::thread::id thread_id, size_t size) {
  if (RestrictPinnedMemUsage())
    NVJpegMem::instance().AddBuffer<mm::memory_kind::host>(thread_id, size);
  else
    NVJpegMem::instance().AddBuffer<mm::memory_kind::pinned>(thread_id, size);
}

/*
template <typename MemoryKind>
void *GetBuffer(std::thread::id thread_id, size_t size) {
  return NVJpegMem::instance().GetBuffer<MemoryKind>(thread_id, size);
}
This function is implemented in memory_pool.cc
*/

void *GetHostBuffer(std::thread::id thread_id, size_t size) {
  if (RestrictPinnedMemUsage())
    return NVJpegMem::instance().GetBuffer<mm::memory_kind::host>(thread_id, size);
  else
    return NVJpegMem::instance().GetBuffer<mm::memory_kind::pinned>(thread_id, size);
}

void DeleteAllBuffers(std::thread::id thread_id) {
  NVJpegMem::instance().DeleteAllBuffers(thread_id);
}

template <typename MemoryKind>
void AddMemStats(std::thread::id thread_id, size_t size) {
  NVJpegMem::instance().AddBuffer<MemoryKind>(thread_id, size);
}

void SetEnableMemStats(bool enabled) {
  NVJpegMem::instance().SetEnableMemStats(enabled);
}

void PrintMemStats() {
  NVJpegMem::instance().PrintMemStats();
}

template void AddBuffer<mm::memory_kind::device>(std::thread::id thread_id, size_t size);
template void AddBuffer<mm::memory_kind::pinned>(std::thread::id thread_id, size_t size);

nvjpegDevAllocator_t GetDeviceAllocator() {
  nvjpegDevAllocator_t allocator;
  allocator.dev_malloc = &DeviceNew;
  allocator.dev_free = &ReturnBufferToPool;
  return allocator;
}

nvjpegPinnedAllocator_t GetPinnedAllocator() {
  nvjpegPinnedAllocator_t allocator;
  allocator.pinned_malloc = &HostNew;
  allocator.pinned_free = &ReturnBufferToPool;
  return allocator;
}

}  // namespace nvjpeg_memory

}  // namespace imgcodec

}  // namespace dali
