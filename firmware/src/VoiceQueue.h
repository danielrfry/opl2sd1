#pragma once

#include <cstdint>

template <int capacity>
class VoiceQueue {
public:
    VoiceQueue()
    {
        this->clear();
    };

    void clear()
    {
        for (int i = 0; i < capacity; i++) {
            contents[i] = -1;
        }
        this->size = 0;
    }

    int8_t popFront()
    {
        if (this->size == 0) {
            return -1;
        }
        int8_t v = this->contents[0];
        for (int i = 0; i < this->size - 1; i++) {
            contents[i] = contents[i + 1];
        }
        contents[this->size - 1] = -1;
        this->size--;
        return v;
    }

    void pushFront(uint8_t v)
    {
        if (this->size == capacity) {
            return;
        }
        for (int i = 1; i < capacity; i++) {
            this->contents[i] = this->contents[i - 1];
        }
        contents[0] = v;
        this->size++;
    }

    void pushBack(uint8_t v)
    {
        if (this->size == capacity) {
            return;
        }
        contents[this->size] = v;
        this->size++;
    }

    void removeAt(int index)
    {
        if (index < 0 || index >= this->size || this->size == 0) {
            return;
        }
        for (int i = index; i < this->size - 1; i++) {
            this->contents[i] = this->contents[i + 1];
        }
        this->contents[this->size - 1] = -1;
        this->size--;
    }

    void remove(uint8_t v)
    {
        for (int i = 0; i < this->size; i++) {
            if (this->contents[i] == v) {
                this->removeAt(i);
                return;
            }
        }
    }

private:
    int8_t contents[capacity];
    int size;
};
