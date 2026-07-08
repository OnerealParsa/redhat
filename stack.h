#ifndef STACK_H
#define STACK_H

#include <vector>

using namespace std;

template <typename T>
class Stack {
private:
    vector<T> items;

public:
    void push(const T& item) { items.push_back(item); }
    T pop() {
        if (items.empty()) return T();
        T item = items.back();
        items.pop_back();
        return item;
    }
    T peek() {
        if (items.empty()) return T();
        return items.back();
    }
    bool is_empty() { return items.empty(); }
    int size() { return items.size(); }
    void clear() { items.clear(); }
};

#endif