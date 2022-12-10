template<typename T, glpaint::usize S>
bool glpaint::RingQueue<T, S>::emplace(const T& value) {
    std::lock_guard<std::mutex> lock{m_};
    if (head_ != tail_ || empty_) {
        empty_ = false;
        queue_[tail_] = value;
        tail_ = (tail_ + 1) % S;

        wait_var_.notify_one();

        return true;
    }
    return false;
}
template<typename T, glpaint::usize S>
bool glpaint::RingQueue<T, S>::emplace(T&& value) {
    std::lock_guard<std::mutex> lock{m_};
    if (head_ != tail_ || empty_) {
        empty_ = false;
        queue_[tail_] = std::move(value);
        tail_ = (tail_ + 1) % S;

        wait_var_.notify_one();

        return true;
    }
    return false;
}
template<typename T, glpaint::usize S>
std::optional<T> glpaint::RingQueue<T, S>::waitingPop() {
    std::unique_lock<std::mutex> lock{m_};
    wait_var_.wait(lock, [&empty = empty_] { return !empty; });

    if (head_ != tail_ && !empty_) {
        const auto previous_head = head_;
        head_ = (head_ + 1) % S;
        if (head_ == tail_) {
            empty_ = true;
        }
        return std::optional(std::move(queue_[previous_head]));
    }

    return std::nullopt;
}
template<typename T, glpaint::usize S>
bool glpaint::RingQueue<T, S>::waitingPop(T& value) {
    std::unique_lock<std::mutex> lock{m_};
    wait_var_.wait(lock, [&empty = empty_] { return !empty; });

    if (head_ != tail_ && !empty_) {
        const auto previous_head = head_;
        head_ = (head_ + 1) % S;
        if (head_ == tail_) {
            empty_ = true;
        }
        if constexpr(std::is_move_assignable_v<T>) {
            value = std::move(queue_[previous_head]);
        } else {
            value = queue_[previous_head];
        }

        return true;
    }
    return false;
}
template<typename T, glpaint::usize S>
std::optional<T> glpaint::RingQueue<T, S>::waitingPop(glpaint::u32 miliseconds) {
    std::unique_lock<std::mutex> lock{m_};
    wait_var_.wait(
        lock, 
        std::chrono::duration<glpaint::u32, std::milli>{miliseconds},
        [&empty = empty_] { return !empty; }
    );

    if (head_ != tail_ && !empty_) {
        const auto previous_head = head_;
        head_ = (head_ + 1) % S;
        if (head_ == tail_) {
            empty_ = true;
        }
        return queue_[previous_head];
    }

    return std::nullopt;
}
template<typename T, glpaint::usize S>
bool glpaint::RingQueue<T, S>::waitingPop(T& value, glpaint::u32 miliseconds) {
    std::unique_lock<std::mutex> lock{m_};
    wait_var_.wait(
        lock, 
        std::chrono::duration<glpaint::u32, std::milli>{miliseconds},
        [&empty = empty_] { return !empty; }
    );

    if (head_ != tail_ && !empty_) {
        const auto previous_head = head_;
        head_ = (head_ + 1) % S;
        if (head_ == tail_) {
            empty_ = true;
        }
        if constexpr(std::is_move_assignable_v<T>) {
            value = std::move(queue_[previous_head]);
        } else {
            value = queue_[previous_head];
        }

        return true;
    }
    return false;
}
template<typename T, glpaint::usize S>
std::optional<T> glpaint::RingQueue<T, S>::pop() {
    std::unique_lock<std::mutex> lock{m_};

    if (head_ != tail_ && !empty_) {
        const auto previous_head = head_;
        head_ = (head_ + 1) % S;
        if (head_ == tail_) {
            empty_ = true;
        }
        return queue_[previous_head];
    }

    return std::nullopt;
}
template<typename T, glpaint::usize S>
bool glpaint::RingQueue<T, S>::pop(T& value) {
    std::unique_lock<std::mutex> lock{m_};

    if (head_ != tail_ && !empty_) {
        const auto previous_head = head_;
        head_ = (head_ + 1) % S;
        if (head_ == tail_) {
            empty_ = true;
        }
        if constexpr(std::is_move_assignable_v<T>) {
            value = std::move(queue_[previous_head]);
        } else {
            value = queue_[previous_head];
        }
        return true;
    }
    return false;
}