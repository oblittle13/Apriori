import os
from apriori_common import is_frequent, PROJECT_DIR, save_frequent_itemsets

def generate_candidate_regular(frequent_itemsets):
    candidates = set()
    for itemset1 in frequent_itemsets:
        for itemset2 in frequent_itemsets:
            candidate = frozenset(sorted(itemset1 | itemset2))
            if len(candidate) == len(itemset1) + 1:
                candidates.add(candidate)
    return candidates

def apriori_regular(db_file, transactions, min_support):
    print("Running Apriori Regular...")

    items = set(item for transaction in transactions for item in transaction)
    frequent_itemsets = {frozenset([item]) for item in items if is_frequent([item], transactions, min_support)}

    k = 1
    all_frequent_itemsets = {}
    num_scans = 0
    while frequent_itemsets:
        # report progress 
        print(f"Found {len(frequent_itemsets)} frequent itemsets of size {k}")

        all_frequent_itemsets[k] = frequent_itemsets
        candidates = generate_candidate_regular(frequent_itemsets)
        
        new_frequent_itemsets = set()
        for itemset in candidates:
            if is_frequent(itemset, transactions, min_support):
                new_frequent_itemsets.add(itemset)
            num_scans += len(transactions)  
        
        frequent_itemsets = new_frequent_itemsets
        k += 1

    db_name = db_file.split(".")[0]
    save_frequent_itemsets(db_name, "AprioriReg", all_frequent_itemsets)
        
    return all_frequent_itemsets, num_scans