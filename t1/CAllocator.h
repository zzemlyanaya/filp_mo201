//
// Created by Evgeniya Zemlyanaya on 19.02.2023.
//

#ifndef FILP_MO201_CALLOCATOR_H
#define FILP_MO201_CALLOCATOR_H

#include <cstdio>
#include <cstdlib>
#include <memory>

template <class T, class Allocator = std::allocator<T> >
class CAllocator {
public:
    typedef T value_type;
    typedef size_t size_type;

    CAllocator();
    CAllocator(CAllocator& other);

    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n);

};


#endif //FILP_MO201_CALLOCATOR_H
