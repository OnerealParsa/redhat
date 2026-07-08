#ifndef BST_H
#define BST_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

struct BSTNode {
    string username;
    int score;
    BSTNode* left;
    BSTNode* right;
    BSTNode(const string& u, int s) : username(u), score(s), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;
    void insert_helper(BSTNode* node, const string& username, int score);
    int search_helper(BSTNode* node, const string& username);
    BSTNode* find_node(BSTNode* node, const string& username);
    void inorder_helper(BSTNode* node, vector<pair<string, int>>& result);
    void destroy(BSTNode* node);

public:
    BST();
    ~BST();
    void insert(const string& username, int score);
    int search(const string& username);
    bool contains(const string& username);
    bool update_score(const string& username, int new_score);
    vector<pair<string, int>> inorder();
    vector<pair<string, int>> get_all_users();
};

#endif