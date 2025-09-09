// SPDX-License-Identifier: Apache-2.0
#include "quasar/thread_pool.hpp"

namespace quasar {

thread_pool::thread_pool(size_t nthreads) {
  if (nthreads == 0) nthreads = std::max<size_t>(1, std::thread::hardware_concurrency());
  threads_.reserve(nthreads);
  for (size_t i = 0; i < nthreads; ++i) threads_.emplace_back([this]{ worker(); });
}

thread_pool::~thread_pool() {
  {
    std::lock_guard<std::mutex> lk(mu_);
    stop_ = true;
  }
  cv_.notify_all();
  for (auto& t : threads_) if (t.joinable()) t.join();
}

void thread_pool::submit(std::function<void()> fn) {
  {
    std::lock_guard<std::mutex> lk(mu_);
    q_.push(std::move(fn));
  }
  cv_.notify_one();
}

void thread_pool::wait_idle() {
  std::unique_lock<std::mutex> lk(mu_);
  cv_.wait(lk, [this]{ return q_.empty() && active_.load() == 0; });
}

void thread_pool::worker() {
  for (;;) {
    std::function<void()> fn;
    {
      std::unique_lock<std::mutex> lk(mu_);
      cv_.wait(lk, [this]{ return stop_ || !q_.empty(); });
      if (stop_ && q_.empty()) return;
      fn = std::move(q_.front());
      q_.pop();
      active_.fetch_add(1);
    }
    fn();
    {
      std::lock_guard<std::mutex> lk(mu_);
      active_.fetch_sub(1);
      if (q_.empty() && active_.load() == 0) cv_.notify_all();
    }
  }
}

} // namespace quasar