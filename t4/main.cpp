//
// Created by Evgeniya Zemlyanaya on 29.03.2023.
//

#include <iostream>
#include <map>
#include <mutex>
#include "FilePtr.h"

using namespace std;

FilePtr cache(const char* filename) {
    static map<const char*, FilePtr> files;
    static mutex m;

   lock_guard guard(m);

   if (files.find(filename) != files.end()) {
       return files.at(filename);
   } else {
        FilePtr file(filename, "r");
       files.insert(make_pair(filename, file));
       return file;
   }
}

int main() {
    FilePtr file = cache("testfile.txt");

    if (!file.isOpen()) {
        cout << "Error: couldn't open the file";
        return 1;
    }

//    while (!file.isEOF()) {
//        cout << file.readLine() << endl;
//    }
    while (!file.isEOF()) {
        cout << file.readWholeLine();
    }
}