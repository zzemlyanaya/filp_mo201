//
// Created by Evgeniya Zemlyanaya on 11.03.2023.
//

#ifndef FILP_MO201_LINKEDLISTMEMORYMANAGER_H
#define FILP_MO201_LINKEDLISTMEMORYMANAGER_H

#pragma once

#include <new>

#pragma pack(push, 1)
struct Block {
    std::size_t size;
    union {
        struct {
            Block* next;
            Block* prev;
        };

        char data[1];
    };
};
#pragma pack(pop)

class LinkedListMemoryManager {
private:
    Block* head;
    const std::size_t bufferSize = 1024 * 1024;

    void insertBlock(Block* block) {
        if (head == nullptr) {
            head = block;
            head->next = head->prev = head;
            return;
        }

        Block* next = head->next;
        head->next = block;
        next->prev = block;
        block->next = next;
        block->prev = head;
    }

    void removeBlock(Block* block) {
        if (block->next == block) {
            head = nullptr;
            return;
        }

        Block* prev = block->prev;
        Block* next = block->next;
        prev->next = next;
        next->prev = prev;

        if (head == block) head = next;
    }

    void mergeFreeBlocks(Block* block) {
        Block* left = nullptr;
        Block* right = nullptr;

        Block* current = head;
        do {
            if ((Block*)((char*)current + current->size) == block)
                left = current;

            if ((Block*)((char*)block + block->size) == current)
                right = current;

            current = current->next;
        } while (current != head);

        if (left != nullptr) {
            removeBlock(block);
            left->size += block->size;
            block = left;
        }

        if (right != nullptr) {
            removeBlock(right);
            block->size += right->size;
        }
    }

public:
    LinkedListMemoryManager() {
        char* buffer = new char[bufferSize];
        head = (Block*)buffer;
        head->size = bufferSize - sizeof(std::size_t);
        head->next = head->prev = head;
    }

    ~LinkedListMemoryManager() {
        delete[] head;
    }

    void* allocate(std::size_t size) {
        if (head == nullptr) throw std::bad_alloc();

        std::size_t requiredSize = size + sizeof(std::size_t);
        Block* current = head;

        while (true) {
            if (current->size >= requiredSize) break;

            current = current->next;
            if (current == head) throw std::bad_alloc();
        }
        removeBlock(current);

        if (current->size >= requiredSize + sizeof(Block)) {
            Block* half = (Block*)((char*)current + requiredSize);
            half->size = current->size - requiredSize;
            current->size = requiredSize;
            insertBlock(half);
        }

        return current->data;
    }

    void free(void* ptr) {
        Block* block = (Block*)((char*)ptr - sizeof(std::size_t));
        insertBlock(block);
        mergeFreeBlocks(block);
    }

};

#endif //FILP_MO201_LINKEDLISTMEMORYMANAGER_H
