#pragma once

#include <queue>
#include <mutex>
#include <optional>
#include <condition_variable>

template<typename T>
class EventQueue {
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;

    bool stopped_ = false;

public:
    void push(T value) {
        {
            std::lock_guard lock(mutex_);
            queue_.push(std::move(value));
        }

        cv_.notify_one();
    }

    std::optional<T> pop() {
        std::unique_lock lock(mutex_);

        cv_.wait(lock, [&] {
            return stopped_ || !queue_.empty();
        });

        if (stopped_ && queue_.empty()) {
            return std::nullopt;
        }

        T value = std::move(queue_.front());

        queue_.pop();

        return value;
    }

    void stop() {
        {
            std::lock_guard lock(mutex_);
            stopped_ = true;
        }

        cv_.notify_all();
    }
};