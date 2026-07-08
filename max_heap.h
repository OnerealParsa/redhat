#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include <vector>
#include <string>
#include <utility>

using namespace std;

class MaxHeap {
private:
    vector<pair<int, string>> heap;
    int parent(int i);
    int left_child(int i);
    int right_child(int i);
    void swap_nodes(int i, int j);
    void sift_up(int i);
    void sift_down(int i);

public:
    void push(const string& username, int score);
    pair<int, string> pop();
    pair<int, string> peek();
    int size();
    pair<int, string> get_top();
    void build_heap(const vector<pair<string, int>>& items);
};

#endif