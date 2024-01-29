#include "Generator.h"
#include <iostream>
#include <string>

int main () {

    int sizes[] = {1000, 10000, 50000, 100000};
    string fileNames[] = {"D1K.txt", "D10K.txt", "D50K.txt", "D100K.txt"};

    //remove old .txt files
    for (string file : fileNames) {
        remove(file.c_str());
    }

    // create new .txt files
    for (int size : sizes ) {
        create_database(size, 99);
    }

    return 0;

}