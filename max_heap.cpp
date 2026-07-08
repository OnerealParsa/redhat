#include "max_heap.h"

int MaxHeap::parent(int i) { return (i - 1) / 2; }
int MaxHeap::left_child(int i) { return 2 * i + 1; }
int MaxHeap::right_child(int i) { return 2 * i + 2; }

void MaxHeap::swap_nodes(int i, int j) {
    auto temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void MaxHeap::push(const string& username, int score) {
    heap.push_back({score, username});
    sift_up(heap.size() - 1);
}

void MaxHeap::sift_up(int i) {
    while (i > 0 && heap[parent(i)].first < heap[i].first) {
        swap_nodes(i, parent(i));
        i = parent(i);
    }
}

pair<int, string> MaxHeap::pop() {
    if (heap.empty()) return {0, ""};
    if (heap.size() == 1) {
        auto top = heap[0];
        heap.pop_back();
        return top;
    }
    auto top = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    sift_down(0);
    return top;
}

void MaxHeap::sift_down(int i) {
    int n = heap.size();
    int largest = i;
    int left = left_child(i);
    int right = right_child(i);
    if (left < n && heap[left].first > heap[largest].first) largest = left;
    if (right < n && heap[right].first > heap[largest].first) largest = right;
    if (largest != i) {
        swap_nodes(i, largest);
        sift_down(largest);
    }
}

pair<int, string> MaxHeap::peek() {
    if (heap.empty()) return {0, ""};
    return heap[0];
}

int MaxHeap::size() { return heap.size(); }

pair<int, string> MaxHeap::get_top() {
    if (heap.empty()) return {0, ""};
    return pop();
}

void MaxHeap::build_heap(const vector<pair<string, int>>& items) {
    heap.clear();
    for (const auto& item : items) {
        heap.push_back({item.second, item.first});
    }
    int n = heap.size();
    for (int i = (n / 2) - 1; i >= 0; i--) {
        sift_down(i);
    }
}