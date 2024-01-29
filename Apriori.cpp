#include "Apriori.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <iostream>

//global
std::ofstream outFile;

void print();

int main (int argc, char* argv[]) {
    //handle argument inputs
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " database support   (e.g., ./apriori D10K 0.01 for 10k with 0.01 support)" << endl;
        return 1;
    }
    
    // start timer
    auto start = std::chrono::system_clock::now();

    // handle inputs and outputs for reading and writing, set min support
    std::string databaseName = argv[1]; 
    float minSupportRate = std::atof(argv[2]);

    int databaseSize = stoi(databaseName.substr(1, databaseName.length() - 5)) * 1000;
    int minSupp = databaseSize * (minSupportRate); //convert from int to double (e.g., 10 to 0.10)
    string minSupportString;
    minSupportString = to_string(static_cast<int>(minSupportRate*100));


    if (minSupportString.length() == 1)
        minSupportString = '0' + minSupportString;

    string outfile = databaseName + "_Apriori_" + minSupportString + ".txt";
    outFile.open(outfile);

    string infile = databaseName + ".txt";


    read_data(infile);

    // start Apriori, end when frequent list is empty

    generate_candidate_1();
    generate_frequent_1(minSupp);
    print();

    while(true) {
        generate_candidate();
        prune_candidate();
        scan_transactions();
        generate_frequent();

        if(FREQUENT.empty())
          break;

        print();
    } 
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    cout << "The time spent is " << duration.count() << "s to get frequent items, with " << COUNT << " transaction scans" << std::endl;

    return 0;

}

void print() {
	int width = 12;

	vector<int> itemVector;

	for(auto it = FREQUENT.begin(); it != FREQUENT.end(); it++) {
		itemVector.clear();
		itemVector=it->first;

		for (int i = 0; i < itemVector.size(); ++i) {
			outFile <<"i" << itemVector[i] <<" ";
		}

		outFile << setw(width) << it->second;
		outFile << endl;

	}
}