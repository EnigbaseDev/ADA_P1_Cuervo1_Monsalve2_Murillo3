#ifndef QUEUE_CIRCULAR_HPP
#define QUEUE_CIRCULAR_HPP

#include <stdexcept>
#include <cstddef>

template <typename T>
class QueueCircular {
private:
    T* data;        
    size_t capacity;
    size_t head;       
    size_t tail;     
    size_t count;  

public:
    
    explicit QueueCircular(size_t cap) 
        : capacity(cap), head(0), tail(0), count(0) {
        if (capacity == 0) capacity = 1;
        data = new T[capacity];
    }

    ~QueueCircular() {
        delete[] data;
    }

   
    QueueCircular(const QueueCircular&) = delete;
    QueueCircular& operator=(const QueueCircular&) = delete;

    bool enqueue(const T& value) {
        if (isFull()) return false;
        data[tail] = value;
        tail = (tail + 1) % capacity;
        count++;
        return true;
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Error: Subflujo en cola circular vacía.");
        }
        T item = data[head];
        head = (head + 1) % capacity;
        count--;
        return item;
    }


    T front() const {
        if (isEmpty()) {
            throw std::underflow_error("Error: Cola circular vacía al consultar frente.");
        }
        return data[head];
    }

    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == capacity; }
    size_t size() const { return count; }
    size_t getCapacity() const { return capacity; }
};

#endif