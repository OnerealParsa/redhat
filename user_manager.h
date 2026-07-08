#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "hash_table.h"
#include "bst.h"
#include "max_heap.h"
#include <string>
#include <utility>
#include <vector>

using namespace std;

class UserManager {
private:
    string data_file;
    HashTable users;
    BST bst;
    void load();
    void save();
    string hash_password(const string& password);

public:
    UserManager(const string& file = "users.txt");
    bool user_exists(const string& username);
    bool verify_password(const string& username, const string& password);
    pair<bool, string> create_user(const string& username, const string& password);
    pair<bool, string> authenticate(const string& username, const string& password);
    bool update_score(const string& username, int new_score);
    int get_user_score(const string& username);
    pair<string, int> get_top_player();
    vector<pair<string, int>> get_all_users();
};

#endif