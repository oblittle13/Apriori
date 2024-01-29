#!/usr/bin/env python3

from apriori_common import load_transactions, PROJECT_DIR
from apriori_idea1 import apriori_idea1
#from apriori_regular import apriori_regular
#from apriori_test import test
import time
import argparse
import os

def verify_result_itemsets(our_itemsets, ref_itemsets):
    for k in our_itemsets:
        our_set = our_itemsets[k]
        ref_set = set(frozenset(itemset) for itemset in ref_itemsets[k])

        if our_set != ref_set:
            print(f"Difference detected in {k}-itemsets:")
            print("In our implementation but not in reference:")
            print(our_set - ref_set)
            print("In reference but not in our implementation:")
            print(ref_set - our_set)
            return False

    print("The results match!")

if __name__ == "__main__":
    # use argparse to get arguments for minimum support and db
    parser = argparse.ArgumentParser()
    parser.add_argument("db", help="the database to run apriori on")
    parser.add_argument("min_support", help="the minimum support for the apriori algorithm")
    args = parser.parse_args()

    # get minsupport and load transactions using params
    min_support = float(args.min_support)
    transactions = load_transactions((args.db + ".txt"))

    # run idea1 and time it
    start_idea1 = time.time()
    itemsets_idea1, num_tx_scan_idea1 = apriori_idea1(args.db, transactions, min_support)
    end_idea1 = time.time()
    num_db_scan_idea1 = num_tx_scan_idea1 / len(transactions)

    # run reference implementation and time it
    # start_test = time.time()
    # ref_itemsets = test(args.db, transactions, min_support)
    # end_test = time.time()

    # print results
    print(f"*** Results from processing {args.db} ***")
    print("")
    print(f"Idea1:")
    print(f"    Time: {end_idea1 - start_idea1}")
    print(f"    Db Scans: {num_db_scan_idea1} (Transactions Scanned: {num_tx_scan_idea1})")
    print("")

    # print(f"Test:")
    # print(f"    Time: {end_test - start_test}")
    # print("")

    # print("Comparing idea1 to reference...")
    # verify_result_itemsets(itemsets_idea1, ref_itemsets)
    # print("")

    # write times to labeled csv
    with open(os.path.join(PROJECT_DIR, "performance.csv"), "a+") as f:
        # headers
        if f.tell() == 0:
            f.write("db,min_support,idea1_time,idea1_scans,test_time\n")
        f.write(f"{args.db},{min_support},{end_idea1 - start_idea1},{num_db_scan_idea1},\n") # add {end_test - start_test}

                