import os.path

PROJECT_DIR = os.path.dirname(os.path.abspath(__file__))

# reads transactions from the given db file and parses it into a nested list of itemssets representing the transactions
def load_transactions(db_name):
    transactions = []

    with open(os.path.join(PROJECT_DIR, db_name), "r") as file:
        for line in file:
            transaction = line.strip().split(', ')
            transactions.append(transaction)

    print(f"Loaded {len(transactions)} transactions from {db_name}.")

    return transactions

# returns true if the given itemset is frequent according to the given min_support in the given transactions
def is_frequent(itemset, transactions, min_support):
    count = sum(1 for transaction in transactions if set(itemset).issubset(transaction))
    return count / len(transactions) >= min_support

# saves the given frequent itemsets to a file labeled with the given db_name and algorithm_used
def save_frequent_itemsets(db_name, algorithm_used, all_frequent_itemsets):
    with open(os.path.join(PROJECT_DIR, f"{db_name}_{algorithm_used}_1.freq"), "w") as outfile:
        for k, itemsets in sorted(all_frequent_itemsets.items()):
            outfile.write(f"{k}-itemsets: {len(itemsets)}\n")
            for itemset in sorted(itemsets, key=lambda x: sorted(x)):
                outfile.write(f"  {sorted(itemset)}\n")