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

add_executable(random random.cpp)

add_executable(ka
        ka/in.txt ka/out.txt ka/1176.cpp)
