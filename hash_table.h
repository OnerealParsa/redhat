#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <string>
#include <utility>

using namespace std;

struct UserData {
    string password_hash;
    int score;
};

class HashTable {
private:
    int size;
    vector<vector<pair<string, UserData>>> table;
    int hash(const string& key);

public:
    HashTable(int sz = 100);
    void insert(const string& key, const UserData& value);
    UserData* get(const string& key);
    bool remove(const string& key);
    bool contains(const string& key);
    vector<string> keys();
};

#endif