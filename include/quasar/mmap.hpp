// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <cstddef>
#include <cstdint>
#include <filesystem>

namespace quasar::mmap {

struct View {
  const char* data = nullptr;
  size_t size = 0;
  void* handle = nullptr; // OS specific
  void* mapping = nullptr;
};

bool map_file(const std::filesystem::path& p, View& view);
void unmap(View& view);

} // namespace quasar::mmap