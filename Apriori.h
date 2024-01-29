#ifndef APRIORI_H
#define APRIORI_H 

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int MIN_SUPP;
int COUNT = 0;
map<vector<int>, int> CANDIDATE;
map<vector<int>, int> FREQUENT;
vector<vector<int>> TRANSACTIONS;

//function to read data
void read_data(string);

//functions for algorithm
void generate_candidate_1();
void generate_frequent_1(int);
void generate_candidate();
void prune_candidate();
void scan_transactions();
void generate_frequent();

// helper functions
bool same_prefix(const vector<int>& , const vector<int>&);
void count_support(const vector<int> &);


void read_data(string fileName) {

    ifstream input(fileName);

    if (!input.is_open()) {
        cerr << "cant open the file : " << fileName << endl;
        exit(0);
    }

    string readLine;

    while(getline(input, readLine)) {
        istringstream ss(readLine);
        vector<int> nums;
        string item;

        while (ss >> item) {
            string temp = item.substr(1); // remove prefix

			if (temp.back() == ',')
				temp = temp.substr(0, temp.size() - 1);
			
            item = ""; //clear string
            nums.push_back(stoi(temp));
        }
		
		sort(nums.begin(), nums.end());
        TRANSACTIONS.push_back(nums);
    }

}

void generate_candidate_1() {

    vector<int> itemVector;

    string line;
    for (const auto &transaction : TRANSACTIONS) {
        for (const auto &item : transaction) {

            itemVector.push_back(item);

			// check each item in transaction and count
            if (CANDIDATE.count(itemVector) > 0) {
                CANDIDATE[itemVector]++;
            } else {
                CANDIDATE[itemVector] = 1;
            }

            itemVector.clear();
        }
    }

}

// used to update min support
void generate_frequent_1(int minSupp) {

	MIN_SUPP = minSupp;

	for(auto it = CANDIDATE.begin();it != CANDIDATE.end();it++) {
		if (it->second >= MIN_SUPP) {
			FREQUENT[it->first] = it->second;
		}
	}
}


void generate_candidate() {

	CANDIDATE.clear();

	for(auto it_1 = FREQUENT.begin(); it_1 != FREQUENT.end(); it_1++) { // current frequent
		for(auto it_2 = it_1; it_2 != FREQUENT.end(); ++it_2) { ; // rest of the frequent list

			vector<int> itemSet, itemSetSuffix;
			itemSet = it_1->first;
			itemSetSuffix = it_2->first;

			if(same_prefix(itemSet, itemSetSuffix)) { // check prefix
				itemSet.push_back(itemSetSuffix.back()); // add last element of itemSetSuffix
				CANDIDATE[itemSet] = 0;
			}
		}
	}
}

//for subsequent generate frequents
void generate_frequent() {

	FREQUENT.clear();

	for(auto it = CANDIDATE.begin();it != CANDIDATE.end();it++) {
		if(it->second >= MIN_SUPP) {
			FREQUENT[it->first] = it->second;
		}
	}
}

//check if they have the same first n - 1 elements (prefix) (e.g., 1, 2, 3 & 1, 2, 5 = true)
bool same_prefix(const vector<int>& a, const vector<int>&  b) {

	for (int i = 0; i < a.size() - 1; i++) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}

void prune_candidate() {

	vector<int> candidateItemSet, itemSetKey;
	
	// loop through each candidate, mark -1 if not in prior frequent itemsets
  	for(auto it = CANDIDATE.begin();it != CANDIDATE.end();it++) {
		itemSetKey.clear();
		candidateItemSet.clear();

		candidateItemSet = it->first;

		//create list with all excluding first element
		for(int i = 1; i < candidateItemSet.size(); i++) {
			itemSetKey.push_back(candidateItemSet[i]);
		}

		//check if its in frequent n -1 items list, if not, mark
		if(FREQUENT.find(itemSetKey) == FREQUENT.end()) { 
			it->second = -1; // mark candidate to be removed
			break;
		}		
	
	}

	//replace candidate with pruned candidate
	map<vector<int>, int> prunedCandidate;

  	for(auto it = CANDIDATE.begin(); it != CANDIDATE.end(); it++) {
		if (it->second != -1) { // skip if marked as -1
			prunedCandidate[it->first] = it->second; // add to new candidate
		}
	}
	
	CANDIDATE.clear();
	CANDIDATE = prunedCandidate;
}

// for each transaction, count each candidate itemset and update support
void scan_transactions() {
	for(auto& transaction : TRANSACTIONS) {
        count_support(transaction);
		COUNT++;
	}
}

void count_support(const vector<int> &transaction) {

	//check transaction against each item in candidate
  	for(auto it = CANDIDATE.begin(); it != CANDIDATE.end(); it++) {

		int count = 0;
        vector<int> candidateItemSet;
		candidateItemSet = it->first;
        int i = 0, j = 0; //create index for items

		//count each item in candidate against transaction
        while (i < candidateItemSet.size() && j < transaction.size()) {

            if (candidateItemSet[i] == transaction[j]) { 
                count++;
                i++;
                j++;
            } else if(candidateItemSet[i] > transaction[j]) {
                j++;
            } else {   // if candidate < transaction,
                break; // break out of while loop,
				       // its sorted, so the candidate will not be frequent for this transaction
            }
        }
		
		// if each item in candidate was in transaction, increment candidate
		// ie, if counted 5 items in candidate list, and it has 5 total items, then + 1 support
		if (count == candidateItemSet.size()) { 
			it->second++;
		}
	}
}

#endif // APRIORI_H