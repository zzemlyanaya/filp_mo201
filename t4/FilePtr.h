//
// Created by Evgeniya Zemlyanaya on 29.03.2023.
//

#ifndef FILP_MO201_FILEPTR_H
#define FILP_MO201_FILEPTR_H

#pragma once

#include <cstdio>
#include <string>

class FilePtr {
    std::FILE* file = nullptr;
    size_t* counter = nullptr;

    void attach(const FilePtr &other) {
        file = other.file;
        counter = other.counter;
        if (counter) (*counter)++;
    }

    void detach() {
        if (!file) return;

        (*counter)--;
        if (*counter == 0) {
            fclose(file);
            delete counter;
        }
        file = nullptr;
        counter = nullptr;
    }

public:
    explicit FilePtr(const char* filename, char* params) {
        file = fopen(filename, params);
        if (file) counter = new size_t(1);
    }

    FilePtr(const FilePtr &other) {
        attach(other);
    }

    ~FilePtr() {
        detach();
    }

    FilePtr& operator=(const FilePtr& other) {
        if (&other == this) return *this;

        detach();
        attach(other);
        return *this;
    }

    std::string readWholeLine() {
        if (!file) return "";

        const int bufferSize = 3;
        std::string result;
        char buffer[bufferSize];
        buffer[bufferSize-1] = '\0';

        while (fgets(buffer, bufferSize, file)) {
            result += buffer;
            if (buffer[strlen(buffer)-1] == '\n') return result;
        }
        return result;
    }

    std::string readLine() {
        if (!file) return "";

        char buffer[1024];
        fgets(buffer, 1024, file);

        return buffer;
    }

    bool isEOF() {
        return !file || feof(file);
    }

    bool isOpen() {
        return file != nullptr;
    }
};

#endif //FILP_MO201_FILEPTR_H
