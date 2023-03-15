//
// Created by Evgeniya Zemlyanaya on 15.03.2023.
//

#ifndef FILP_MO201_BUDDYMEMORYMANAGER_H
#define FILP_MO201_BUDDYMEMORYMANAGER_H

#pragma once

#include <new>

#pragma pack(push, 1)
struct Block {
    unsigned char level;
    bool isFree;

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

template <class T>
class CList {
    T* head = nullptr;

public:
    void removeItem(T* item) {
        if (item->next == item) {
            head = nullptr;
            return;
        }

        T* prev = item->prev;
        T* next = item->next;
        prev->next = next;
        next->prev = prev;

        if (head == item)
            head = next;
    }

    void insertItem(T* item) {
        if (head == nullptr) {
            head = item;
            head->next = head->prev = head;
            return;
        }

        T* next = head->next;
        head->next = item;
        next->prev = item;
        item->next = next;
        item->prev = head;
    }

    T* getHead() {
        return head;
    }

};

class BuddyMemoryManager {
private:
    static const int MAX_LEVEL = 20;

    const size_t sizeBuffer = 1 << MAX_LEVEL;
    CList<Block> lists[MAX_LEVEL+1];
    char* buffer;

    Block* getBuddy(Block* item) {
        // TODO

    }

public:
    BuddyMemoryManager() {
        buffer = new char[sizeBuffer];
        Block* maxItem = reinterpret_cast<Block*>(buffer);
        maxItem->level = MAX_LEVEL;
        maxItem->isFree = true;

        lists[MAX_LEVEL].insertItem(maxItem);
    }

    ~BuddyMemoryManager() {
        free(buffer);
        free(lists);
    }

    void* allocate(size_t size) {
        size_t requiredSize = size + sizeof(unsigned char) + sizeof(bool);

        for (size_t i = 0; i <= MAX_LEVEL; i++) {
            size_t blockSize = 1 << i;
            if (lists[i].getHead() != nullptr && blockSize >= size) {
                Block* block = lists[i].getHead();
                lists[i].removeItem(block);
                block->isFree = false;

                while (true) {
                    if (blockSize / 2 >= requiredSize) {
                        blockSize = blockSize / 2;
                        block->level--;
                        Block* left = block;

                        Block* right = reinterpret_cast<Block*>((char*)block + blockSize);
                        right->level = block->level;
                        right->isFree = true;

                        lists[right->level].insertItem(right);
                    }
                    else {
                        return block->data;
                    }
                }
            }
        }

        throw std::bad_alloc();
    }

    void free(void* ptr) {
        Block* block = (Block*)((char*)ptr - sizeof(unsigned char)-sizeof(bool));

    }

};

#endif //FILP_MO201_BUDDYMEMORYMANAGER_H
