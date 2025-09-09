// SPDX-License-Identifier: Apache-2.0
#include "quasar/mmap.hpp"
#include <cstdio>
#include <system_error>
#include <fstream>

#if defined(_WIN32)
  #include <windows.h>
#else
  #include <sys/mman.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
#endif

namespace quasar::mmap {

bool map_file(const std::filesystem::path& p, View& view) {
#if defined(_WIN32)
  HANDLE h = CreateFileW(p.wstring().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (h == INVALID_HANDLE_VALUE) return false;
  LARGE_INTEGER sz;
  if (!GetFileSizeEx(h, &sz)) { CloseHandle(h); return false; }
  if (sz.QuadPart == 0) { CloseHandle(h); return false; }
  HANDLE m = CreateFileMappingW(h, NULL, PAGE_READONLY, 0, 0, NULL);
  if (!m) { CloseHandle(h); return false; }
  void* addr = MapViewOfFile(m, FILE_MAP_READ, 0, 0, 0);
  if (!addr) { CloseHandle(m); CloseHandle(h); return false; }
  view.handle = h;
  view.mapping = m;
  view.data = static_cast<const char*>(addr);
  view.size = static_cast<size_t>(sz.QuadPart);
  return true;
#else
  int fd = ::open(p.c_str(), O_RDONLY);
  if (fd < 0) return false;
  struct stat st;
  if (fstat(fd, &st) != 0) { ::close(fd); return false; }
  size_t sz = static_cast<size_t>(st.st_size);
  if (sz == 0) { ::close(fd); return false; }
  void* addr = ::mmap(nullptr, sz, PROT_READ, MAP_PRIVATE, fd, 0);
  if (addr == MAP_FAILED) { ::close(fd); return false; }
  view.handle = reinterpret_cast<void*>(static_cast<intptr_t>(fd));
  view.mapping = nullptr;
  view.data = static_cast<const char*>(addr);
  view.size = sz;
  return true;
#endif
}

void unmap(View& view) {
#if defined(_WIN32)
  if (view.data) UnmapViewOfFile(view.data);
  if (view.mapping) CloseHandle((HANDLE)view.mapping);
  if (view.handle) CloseHandle((HANDLE)view.handle);
#else
  if (view.data && view.size) munmap((void*)view.data, view.size);
  if (view.handle) ::close(static_cast<int>(reinterpret_cast<intptr_t>(view.handle)));
#endif
  view = View{};
}

} // namespace quasar::mmap