#include "ratelimiter.hpp"

RateLimiter::RateLimiter(size_t C, uint64_t T, size_t L)
    : max_capacity_C(C),
      time_window_T(T),
      max_packets_L(L),
      buffer_queue(C),
      total_accepted(0),
      total_rejected_buffer(0),
      total_rejected_rate(0),
      max_buffer_occupancy(0) {}

void RateLimiter::purgeExpired(uint64_t current_timestamp) {
    // timestamp_queue y buffer_queue crecen siempre en sincronia: cada paquete
    // ACEPTADO se encola en ambas al mismo tiempo (ver processPacket). Por lo
    // tanto, al purgar de timestamp_queue una marca de tiempo que ya salio de
    // la ventana T, el paquete correspondiente en buffer_queue se considera
    // procesado y tambien se libera. Sin este dequeue, buffer_queue nunca se
    // vacia: se llena una vez con los primeros C paquetes y rechaza todo el
    // trafico restante para siempre, lo cual no representa un firewall real.
    while (!timestamp_queue.isEmpty()) {
        uint64_t oldest_ts = timestamp_queue.front();
        if (current_timestamp >= oldest_ts + time_window_T) {
            timestamp_queue.dequeue();
            buffer_queue.dequeue();
        } else {
            break;
        }
    }
}

PacketResult RateLimiter::processPacket(uint64_t timestamp_ms, size_t size_bytes) {
    // 1. Limpiar marcas de tiempo fuera de la ventana T
    purgeExpired(timestamp_ms);

    // 2. Evaluar limite de capacidad (Buffer)
    if (buffer_queue.size() >= max_capacity_C) {
        total_rejected_buffer++;
        return {timestamp_ms, size_bytes, Decision::REJECTED_BUFFER_FULL};
    }

    // 3. Evaluar limite de tasa (Rate Limit)
    if (timestamp_queue.size() >= max_packets_L) {
        total_rejected_rate++;
        return {timestamp_ms, size_bytes, Decision::REJECTED_RATE_LIMIT};
    }

    // 4. Aceptar paquete
    buffer_queue.enqueue(size_bytes);
    timestamp_queue.enqueue(timestamp_ms);

    total_accepted++;
    if (buffer_queue.size() > max_buffer_occupancy) {
        max_buffer_occupancy = buffer_queue.size();
    }

    return {timestamp_ms, size_bytes, Decision::ACCEPTED};
}