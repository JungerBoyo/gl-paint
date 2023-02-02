#ifndef GLPAINT_RING_QUEUE_HPP
#define GLPAINT_RING_QUEUE_HPP

#include <cinttypes>
#include <array>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <optional>

#include <common.hpp>

namespace glpaint {

template<typename T, usize S> struct RingQueue {
private:
    std::mutex m_;
    std::condition_variable wait_var_;

    std::array<T, S> queue_;
    usize head_{ 0 };
    usize tail_{ 0 };
    bool empty_{ true };
public:
    RingQueue() = default;
    RingQueue(const RingQueue<T, S>& other) = delete;
    RingQueue(RingQueue<T, S>&& other) = delete;    
    RingQueue<T, S>& operator=(const RingQueue<T, S>& other) = delete;
    RingQueue<T, S>& operator=(RingQueue<T, S>&& other) = delete;    
    ~RingQueue() = default;
    
    bool emplace(const T& value);
    bool emplace(T&& value);
    std::optional<T> waitingPop();
    bool waitingPop(T& value); 
    std::optional<T> waitingPop(u32 miliseconds);
    bool waitingPop(T& value, u32 miliseconds);
    std::optional<T> pop();    
    bool pop(T& value);
};
}

#include "RingQueue.tpp"

#endif