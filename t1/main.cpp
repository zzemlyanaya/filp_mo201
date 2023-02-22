#include <iostream>
#include <fstream>
#include <map>
#include <chrono>
#include "CComparator.h"
#include "CAllocator.h"
#include "PoolAllocator.cpp"

using namespace std;

bool isDelim(char &c) {
    switch (c) {
        case ' ':
        case '\n':
        case '.':
        case ',':
        case '!':
        case '-':
        case ';':
        case ':':
        case '?':
        case '"':
        case '\'':
        case '(':
        case ')':
        case '[':
        case ']':
        case '/':
            return true;
        default:
            return false;
    }
}

char* getText() {
    size_t size;
    ifstream inputFile("book.txt", ios::binary);

    inputFile.seekg(0, ios::end);
    size = inputFile.tellg();
    inputFile.seekg(0);

//    cout << size << endl;

    char* text = new char[size+1];
    inputFile.read(text, size);
    inputFile.close();

    return text;
}

size_t countWords(char* text) {
    size_t words = 0;
    char *p = text;
    bool prevSpace = true;

    while (*p) {
        bool isDel = isDelim(*p);
        if (prevSpace && !isDel) words++;
        prevSpace = isDel;
        p++;
    }

    return words;
}

void doMagicManually(char* text) {
    map<char*, size_t, CComparator, PoolAllocator<char*const>> map;
    char *start = text;

    size_t words = countWords(text);

    for (int i = 0; i < words; ++i) {
        while (isDelim(*start)) start++;
        char *end = start + 1;
        while (*end && !isDelim(*end)) end++;
        end[0] = '\0';
        end++;

        auto it = map.find(start);
        if (it == map.end()){
            map.insert(make_pair(start, 1));
        } else {
            it->second++;
        }

        start = end;
    }

//    for(auto it : map){
//        cout << it.first << " " << it.second << endl;
//    }
}

void doMagicStd(char* text) {
    map<string, size_t> map;
    char *start = text;

    size_t words = countWords(text);

    for (int i = 0; i < words; ++i) {
        while (isDelim(*start)) start++;
        char *end = start + 1;
        while (*end && !isDelim(*end)) end++;
        end[0] = '\0';
        end++;

        auto it = map.find(start);
        if (it == map.end()){
            map.insert(make_pair(string(start, end-start-1), 1));
        } else {
            it->second++;
        }

        start = end;
    }

    for(auto it : map){
        cout << it.first << " " << it.second << endl;
    }
}

int main() {
    char* textChar = getText();

    auto start = chrono::high_resolution_clock::now();
    doMagicManually(textChar);
    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time - manual (ms): " << duration.count() << endl;

    textChar = getText();

    start = chrono::high_resolution_clock::now();
    doMagicStd(textChar);
    stop = chrono::high_resolution_clock::now();

    duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time - std (ms): " << duration.count() << endl;

    delete textChar;

    return 0;
}
