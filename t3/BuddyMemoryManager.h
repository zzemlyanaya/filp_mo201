//
// Created by Evgeniya Zemlyanaya on 15.03.2023.
//

#ifndef FILP_MO201_BUDDYMEMORYMANAGER_H
#define FILP_MO201_BUDDYMEMORYMANAGER_H

#ifdef __LP64__
#define BITS 64
#else
#define BITS 32
#endif

#pragma once

#include <cstdio>

int bsr(size_t x) {
    return BITS-__builtin_clz(x | 1);
}

int bsf(size_t x) {
    return __builtin_ctz(x);
}

#pragma pack(push, 1)
struct Block {
    unsigned char level;
    bool isFree;

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
    size_t mask = 0;

    Block* getBuddy(Block* item) {
        const size_t dist = ((char*)item - buffer);
        const size_t blockSize = 1 << item->level;

        size_t blockNum = dist / blockSize;

        if (blockNum % 2 == 0) {
            return (Block*)((char*)item + blockSize);
        }

        return (Block*)((char*)item - blockSize);
    }

    void insert(Block* block) {
        lists[block->level].insertItem(block);
        block->isFree = true;
        mask = mask | (1 << block->level);
    }

    void remove(Block* block) {
        lists[block->level].removeItem(block);
        block->isFree = false;
        mask = mask & ~(1 << block->level);
    }

public:
    BuddyMemoryManager() {
        buffer = new char[sizeBuffer];
        Block* maxItem = reinterpret_cast<Block*>(buffer);
        maxItem->level = MAX_LEVEL;
        insert(maxItem);
    }

    ~BuddyMemoryManager() {
        free(buffer);
        free(lists);
    }

    void* allocate(size_t size) {
        size_t requiredSize = size + sizeof(unsigned char) + sizeof(bool);

        int x1 = (1 << (bsr(requiredSize)+1)) - 1;
        int x2 = ~x1;
        int i = bsf(mask & x2);

        size_t blockSize = 1 << i;

        if (lists[i].getHead() != nullptr && blockSize >= size) {
            Block* block = lists[i].getHead();
            remove(block);

            while (true) {
                if (blockSize / 2 >= requiredSize) {
                    blockSize = blockSize / 2;
                    block->level--;

                    Block* right = reinterpret_cast<Block*>((char*)block + blockSize);
                    right->level = block->level;

                    insert(right);
                }
                else {
                    return block->data;
                }
            }
        }

        throw std::bad_alloc();
    }

    int free(void* ptr) {
        Block* block = (Block*)((char*)ptr - sizeof(unsigned char)-sizeof(bool));

        while (true) {
            if (block->level == MAX_LEVEL) {
                insert(block);
                break;
            }

            Block* pBuddy = getBuddy(block);

            if (pBuddy->level == block->level && pBuddy->isFree) {
                //lists[pBuddy->level].removeItem(pBuddy);
                remove(pBuddy);

                // Check which one level is less
                if (block > pBuddy) {
                    block = pBuddy;
                }

                block->level++;
            }
            else {
                insert(block);
                break;
            }
        }
    }

};

#endif //FILP_MO201_BUDDYMEMORYMANAGER_H
