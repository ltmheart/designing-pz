#pragma once
#include <vector>
#include "Transaction.h"

// модел
class TransactionModel {
    std::vector<Transaction> items;
public:
    void add(const Transaction& t) { items.push_back(t); }
    int nextId() const { return (int)items.size() + 1; }
};
