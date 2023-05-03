cmake_minimum_required(VERSION 3.24)
project(filp_mo201)

set(CMAKE_CXX_STANDARD 17)

add_executable(t1
        t1/main.cpp
        common/CComparator.h common/CComparator.cpp
        t1/PoolAllocator.cpp
        cmake-build-debug/book.txt)

add_executable(t2
        t2/main.cpp
        common/CComparator.h common/CComparator.cpp
        t2/LinkedListAllocator.h t2/LinkedListMemoryManager.h
        cmake-build-debug/book.txt)

add_executable(t3
        t3/main.cpp
        common/CComparator.h common/CComparator.cpp
        t3/BuddyMemoryManager.h t3/BuddyAllocator.h
        cmake-build-debug/book.txt)

add_executable(t4
        t4/main.cpp t4/FilePtr.h
        cmake-build-debug/book.txt)

add_executable(t5
        t5/main.cpp t5/BinaryTree.h)

add_executable(t6
        t6/main.cpp t6/Treap.h)

add_executable(t7
        t7/main.cpp t7/AVLTree.h)

add_executable(random random.cpp)

add_executable(ka
        ka/in.txt ka/out.txt ka/lab3.cpp)
