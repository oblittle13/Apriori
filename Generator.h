#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <set>
#include <vector>

using namespace std;

string filename;


void generator(int numElements, int maxElement) {
    // create random generator
    std::random_device randomDevice;
    std::mt19937 mt(randomDevice());
    std::uniform_int_distribution<int> dist(0, maxElement);

    // store in set as don't want duplicates & want it to remain sorted
    std::set<int> uniqueItems;

    // Generate unique random elements, using set so continues until full
    while (uniqueItems.size() < numElements) {
        uniqueItems.insert(dist(mt));
    }

    // open text file to add new transaction line for random elements
    std::ofstream outFile;
    outFile.open(filename, std::ios::app); // append mode

    if (outFile.is_open()) {
        for (auto it = uniqueItems.begin(); it != uniqueItems.end(); ++it) {
            outFile << "i" << *it;
            if (std::next(it) != uniqueItems.end()) {
                outFile << ", ";
            }
        }
        outFile << std::endl;
        outFile.close();
    } else {
        std::cerr << "can't open file" << std::endl;
        exit(0);
    }
}


void create_database(int numTransactions, int maxElement, int numElements = 15) {

    filename = 'D' + to_string((numTransactions/1000)) + "K.txt";

    for(int i = 0; i < numTransactions; i++) {

        std::random_device randomDevice;
        std::mt19937 mt(randomDevice());
        std::uniform_int_distribution<int> distribution(5, numElements);

        generator(distribution(mt), maxElement);
    }


}

#endif // GENERATOR_H