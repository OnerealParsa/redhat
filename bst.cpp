#include "bst.h"

BST::BST() : root(nullptr) {}

BST::~BST() { destroy(root); }

void BST::destroy(BSTNode* node) {
    if (node) {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}

void BST::insert(const string& username, int score) {
    if (!root) {
        root = new BSTNode(username, score);
    } else {
        insert_helper(root, username, score);
    }
}

void BST::insert_helper(BSTNode* node, const string& username, int score) {
    if (username < node->username) {
        if (!node->left) {
            node->left = new BSTNode(username, score);
        } else {
            insert_helper(node->left, username, score);
        }
    } else if (username > node->username) {
        if (!node->right) {
            node->right = new BSTNode(username, score);
        } else {
            insert_helper(node->right, username, score);
        }
    } else {
        node->score = score;
    }
}

int BST::search(const string& username) {
    return search_helper(root, username);
}

int BST::search_helper(BSTNode* node, const string& username) {
    if (!node) return -1;
    if (username == node->username) return node->score;
    else if (username < node->username) return search_helper(node->left, username);
    else return search_helper(node->right, username);
}

bool BST::contains(const string& username) {
    return search_helper(root, username) != -1;
}

bool BST::update_score(const string& username, int new_score) {
    BSTNode* node = find_node(root, username);
    if (node) {
        node->score = new_score;
        return true;
    }
    return false;
}

BSTNode* BST::find_node(BSTNode* node, const string& username) {
    if (!node) return nullptr;
    if (username == node->username) return node;
    else if (username < node->username) return find_node(node->left, username);
    else return find_node(node->right, username);
}

vector<pair<string, int>> BST::inorder() {
    vector<pair<string, int>> result;
    inorder_helper(root, result);
    return result;
}

void BST::inorder_helper(BSTNode* node, vector<pair<string, int>>& result) {
    if (node) {
        inorder_helper(node->left, result);
        result.push_back({node->username, node->score});
        inorder_helper(node->right, result);
    }
}

vector<pair<string, int>> BST::get_all_users() {
    return inorder();
}