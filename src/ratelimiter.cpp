#include "ratelimiter.hpp"
#include <algorithm>

RateLimiter::RateLimiter(size_t C, uint64_t T, size_t L)
    : max_capacity_C(C),
      time_window_T(T),
      max_packets_L(L),
      buffer_queue(C),
      timestamp_queue(),
      total_accepted(0),
      total_rejected_buffer(0),
      total_rejected_rate(0),
      max_buffer_occupancy(0) {}

void RateLimiter::purgeExpired(uint64_t current_timestamp) {
    while (!timestamp_queue.isEmpty()) {
        uint64_t oldest = timestamp_queue.front();
        if (current_timestamp >= time_window_T && oldest <= (current_timestamp - time_window_T)) {
            timestamp_queue.dequeue();
        } else {
            break;
        }
    }
}

PacketResult RateLimiter::processPacket(uint64_t timestamp_ms, size_t size_bytes) {
    purgeExpired(timestamp_ms);

    if (timestamp_queue.size() >= max_packets_L) {
        total_rejected_rate++;
        return {timestamp_ms, size_bytes, Decision::REJECTED_RATE_LIMIT};
    }

    if (buffer_queue.isFull()) {
        total_rejected_buffer++;
        return {timestamp_ms, size_bytes, Decision::REJECTED_BUFFER_FULL};
    }

    buffer_queue.enqueue(size_bytes);
    timestamp_queue.enqueue(timestamp_ms);
    total_accepted++;

    max_buffer_occupancy = std::max(max_buffer_occupancy, buffer_queue.size());

    return {timestamp_ms, size_bytes, Decision::ACCEPTED};
}