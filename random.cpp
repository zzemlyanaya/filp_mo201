//
// Created by Evgeniya Zemlyanaya on 15.02.2023.
//

#include <iostream>

#include <cstdio>
#include <vector>
#include <map>

#include <algorithm>

using namespace std;

#ifdef __LP64__
#define BITS 64
#else
#define BITS 32
#endif

int bsr(size_t x) {
    return BITS-__builtin_clz(x | 1);
}

int bsf(size_t x) {
    return __builtin_ctz(x);
}

int main() {

    cout << bsr(12) << endl;
    cout << bsr(100) << endl;
    cout << bsr(24) << endl;
    cout << bsr(16) << endl;
    cout << bsr(128) << endl;

    cout << bsf(12) << endl;
    cout << bsf(100) << endl;
    cout << bsf(24) << endl;
    cout << bsf(16) << endl;
    cout << bsf(128) << endl;
}