import itertools
from apriori_common import is_frequent, save_frequent_itemsets

def generate_candidates_from_transaction_idea1(transaction, k, frequent_itemsets):
    candidates = set()
    k_itemsets = [itemset for itemset in itertools.combinations(transaction, k) if frozenset(itemset) in frequent_itemsets]
    
    for itemset1, itemset2 in itertools.combinations(k_itemsets, 2):
        candidate = frozenset(itemset1).union(itemset2)
        if len(candidate) == k + 1:
            candidates.add(candidate)
    return candidates

def apriori_idea1(db_file, transactions, min_support):
    print("Running Apriori Idea 1...")

    # collect all unique items from transactions into a set of 1-itemsets
    items = set(item for transaction in transactions for item in transaction)

    # filter out infrequent 1-itemsets
    frequent_itemsets = {frozenset([item]) for item in items if is_frequent([item], transactions, min_support)}

    num_scans = 0

    k = 1
    all_frequent_itemsets = {}

    # iterate until frequent itemsets of size k are empty
    while frequent_itemsets:
        # report progress
        print(f"Found {len(frequent_itemsets)} frequent itemsets of size {k}")

        all_frequent_itemsets[k] = frequent_itemsets
        k += 1

        # initalize candidate frequency dict used to track candidate support on the fly
        candidates_frequency = {}

        # generate candidates of size k from each transaction
        for transaction in transactions:
            num_scans += 1
            candidates = generate_candidates_from_transaction_idea1(transaction, k - 1, frequent_itemsets)

            # increment the frequency of each candidate from the transaction
            for candidate in candidates:
                candidates_frequency[candidate] = candidates_frequency.get(candidate, 0) + 1
        
        # filter out infrequent candidates right away
        frequent_itemsets = {itemset for itemset, freq in candidates_frequency.items() if freq / len(transactions) >= min_support}

    db_name = db_file.split(".")[0]

    save_frequent_itemsets(db_name, "Idea1", all_frequent_itemsets)

    return all_frequent_itemsets, num_scans