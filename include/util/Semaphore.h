/*
 * Semaphore.h
 *
 *  Created on: 05 May 2019
 *      Author: Pill
 */

#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <mutex>
#include <condition_variable>

namespace pdesmas {
  class Semaphore {
  public:
    explicit Semaphore(int count = 0) : count_(count) {}

    void Signal() {
      std::unique_lock<std::mutex> lock(mutex_);
      ++count_;
      cv_.notify_one();
    }

    void Wait() {
      std::unique_lock<std::mutex> lock(mutex_);
      cv_.wait(lock, [=] { return count_ > 0; });
      --count_;
    }

    void Reset() {
      std::unique_lock<std::mutex> lock(mutex_);
      count_ = 1;
      cv_.notify_one();
    }

  private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
  };
}
#endif
