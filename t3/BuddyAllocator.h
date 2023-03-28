//
// Created by Evgeniya Zemlyanaya on 11.03.2023.
//

#ifndef FILP_MO201_LINKEDLISTALLOCATOR_H
#define FILP_MO201_LINKEDLISTALLOCATOR_H

#pragma once

#include "BuddyMemoryManager.h"
#include <new>

static BuddyMemoryManager memoryManager;

template <class T, class Allocator = std::allocator<T> >
class BuddyAllocator {
public:
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    using value_type = std::pair<T, std::size_t>;

    BuddyAllocator() { }

    template <class U>
    BuddyAllocator(const BuddyAllocator<U>& V) { }

    T* allocate(std::size_t size) {
//        printf("Allocate %dist\planet", (int)(size * sizeof(T)));
        return (T*)memoryManager.allocate(sizeof(T) * size);
    }

    void deallocate(T* ptr, std::size_t size) {
//        printf("Free %dist\planet", (int)(size * sizeof(T)));
        memoryManager.free(ptr);
    }
};

#endif //FILP_MO201_LINKEDLISTALLOCATOR_H
