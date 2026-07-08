#include "hash_table.h"

HashTable::HashTable(int sz) : size(sz), table(sz) {}

int HashTable::hash(const string& key) {
    unsigned long h = 0;
    for (char c : key) {
        h = h * 31 + (unsigned char)c;
    }
    return h % size;
}

void HashTable::insert(const string& key, const UserData& value) {
    int idx = hash(key);
    for (auto& p : table[idx]) {
        if (p.first == key) {
            p.second = value;
            return;
        }
    }
    table[idx].push_back({key, value});
}

UserData* HashTable::get(const string& key) {
    int idx = hash(key);
    for (auto& p : table[idx]) {
        if (p.first == key) {
            return &p.second;
        }
    }
    return nullptr;
}

bool HashTable::remove(const string& key) {
    int idx = hash(key);
    for (size_t i = 0; i < table[idx].size(); i++) {
        if (table[idx][i].first == key) {
            table[idx].erase(table[idx].begin() + i);
            return true;
        }
    }
    return false;
}

bool HashTable::contains(const string& key) {
    int idx = hash(key);
    for (auto& p : table[idx]) {
        if (p.first == key) return true;
    }
    return false;
}

vector<string> HashTable::keys() {
    vector<string> result;
    for (auto& bucket : table) {
        for (auto& p : bucket) {
            result.push_back(p.first);
        }
    }
    return result;
}