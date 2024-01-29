from apriori_python import apriori
from apriori_common import load_transactions, save_frequent_itemsets

def test(db_file, transactions, min_supp):
    print("Running Apriori Reference...")

    # Run the algorithm
    itemsets, rules = apriori(transactions, min_supp, 0.5)

    # Print the results
    save_frequent_itemsets(db_file, "Reference", itemsets)

    return itemsets