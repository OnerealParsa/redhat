#include "user_manager.h"
#include <fstream>
#include <sstream>
#include <functional>

UserManager::UserManager(const string& file) : data_file(file), users(200) {
    load();
}

string UserManager::hash_password(const string& password) {
    return to_string(hash<string>{}(password));
}

void UserManager::load() {
    ifstream file(data_file);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string username, password_hash;
        int score;
        if (iss >> username >> password_hash >> score) {
            UserData data;
            data.password_hash = password_hash;
            data.score = score;
            users.insert(username, data);
            bst.insert(username, score);
        }
    }
    file.close();
}

void UserManager::save() {
    ofstream file(data_file);
    auto all_users = bst.get_all_users();
    for (const auto& user : all_users) {
        string username = user.first;
        int score = user.second;
        UserData* data = users.get(username);
        if (data) {
            file << username << " " << data->password_hash << " " << score << "\n";
        }
    }
    file.close();
}

bool UserManager::user_exists(const string& username) {
    return bst.contains(username);
}

bool UserManager::verify_password(const string& username, const string& password) {
    UserData* data = users.get(username);
    if (!data) return false;
    return data->password_hash == hash_password(password);
}

pair<bool, string> UserManager::create_user(const string& username, const string& password) {
    if (bst.contains(username)) {
        return {false, "Username already exists."};
    }
    UserData data;
    data.password_hash = hash_password(password);
    data.score = 0;
    users.insert(username, data);
    bst.insert(username, 0);
    save();
    return {true, "Account created successfully."};
}

pair<bool, string> UserManager::authenticate(const string& username, const string& password) {
    if (!bst.contains(username)) {
        return {false, "Username not found."};
    }
    if (!verify_password(username, password)) {
        return {false, "Incorrect password."};
    }
    return {true, "Login successful."};
}

bool UserManager::update_score(const string& username, int new_score) {
    if (!bst.contains(username)) return false;
    bst.update_score(username, new_score);
    UserData* data = users.get(username);
    if (data) {
        data->score = new_score;
    }
    save();
    return true;
}

int UserManager::get_user_score(const string& username) {
    return bst.search(username);
}

pair<string, int> UserManager::get_top_player() {
    auto all_users = bst.get_all_users();
    if (all_users.empty()) return {"", 0};
    MaxHeap heap;
    heap.build_heap(all_users);
    auto top = heap.get_top();
    return {top.second, top.first};
}

vector<pair<string, int>> UserManager::get_all_users() {
    return bst.get_all_users();
}