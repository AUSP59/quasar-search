// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace quasar {

class thread_pool {
 public:
  explicit thread_pool(size_t nthreads);
  ~thread_pool();

  void submit(std::function<void()> fn);
  void wait_idle();

 private:
  void worker();

  std::vector<std::thread> threads_;
  std::mutex mu_;
  std::condition_variable cv_;
  std::queue<std::function<void()>> q_;
  std::atomic<bool> stop_{false};
  std::atomic<size_t> active_{0};
};

} // namespace quasar