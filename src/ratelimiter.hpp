#ifndef RATELIMITER_HPP
#define RATELIMITER_HPP

#include "queue_circular.hpp"
#include "queue_list.hpp"
#include <cstdint>
#include <string>

enum class Decision {
    ACCEPTED,
    REJECTED_BUFFER_FULL,
    REJECTED_RATE_LIMIT
};

struct PacketResult {
    uint64_t timestamp;
    size_t size;
    Decision decision;
};

class RateLimiter {
private:
    size_t max_capacity_C;
    uint64_t time_window_T;
    size_t max_packets_L;

    QueueCircular<size_t> buffer_queue;
    QueueList<uint64_t> timestamp_queue;

    size_t total_accepted;
    size_t total_rejected_buffer;
    size_t total_rejected_rate;
    size_t max_buffer_occupancy;

public:
    RateLimiter(size_t C, uint64_t T, size_t L);
    
    PacketResult processPacket(uint64_t timestamp_ms, size_t size_bytes);
    void purgeExpired(uint64_t current_timestamp);

    size_t getTotalAccepted() const { return total_accepted; }
    size_t getTotalRejectedBuffer() const { return total_rejected_buffer; }
    size_t getTotalRejectedRate() const { return total_rejected_rate; }
    size_t getMaxBufferOccupancy() const { return max_buffer_occupancy; }
    size_t getBufferCount() const { return buffer_queue.size(); }
    size_t getTimestampCount() const { return timestamp_queue.size(); }
    bool isBufferFull() const { return buffer_queue.isFull(); }
    bool isBufferEmpty() const { return buffer_queue.isEmpty(); }
    bool isTimestampQueueEmpty() const { return timestamp_queue.isEmpty(); }
    size_t getBufferCapacity() const { return max_capacity_C; }
};

#endif