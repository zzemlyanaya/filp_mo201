//
// Created by Evgeniya Zemlyanaya on 19.02.2023.
//

#include "CAllocator.h"

template<class T, class Allocator>
CAllocator<T, Allocator>::CAllocator() {

}

template<class T, class Allocator>
CAllocator<T, Allocator>::CAllocator(CAllocator &other) {

}

template<class T, class Allocator>
T *CAllocator<T, Allocator>::allocate(std::size_t n) {
    printf("Allocating %d\n", (int)(n * sizeof(T)));
    return (T*)malloc(sizeof(T) * n);
}

template<class T, class Allocator>
void CAllocator<T, Allocator>::deallocate(T *p, std::size_t n) {
    printf("Freeing %d\n", (int)(n * sizeof(T)));
    free(p);
}