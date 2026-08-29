#ifndef QUEUE_LIST_HPP
#define QUEUE_LIST_HPP

#include <stdexcept>
#include <cstddef>

template <typename T>
class QueueList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t count;

public:
    QueueList() : head(nullptr), tail(nullptr), count(0) {}

    ~QueueList() {
        clear();
    }

    QueueList(const QueueList&) = delete;
    QueueList& operator=(const QueueList&) = delete;

    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    void enqueue(const T& value) {
        Node* newNode = new Node(value);
        if (isEmpty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Error: Subflujo en cola enlazada vacía.");
        }
        Node* temp = head;
        T item = temp->data;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
        count--;
        return item;
    }

    T front() const {
        if (isEmpty()) {
            throw std::underflow_error("Error: Cola enlazada vacía al consultar frente.");
        }
        return head->data;
    }

    bool isEmpty() const { return count == 0; }
    size_t size() const { return count; }
};

#endif